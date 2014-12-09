
--
-- define the extension
--
premake.extensions.compilationunit = {

	--
	-- these are private, do not touch
	--
	enabled = false,
	numcompilationunits = 8

}


--
-- Specify the path, relative to the current script or absolute, where the compilation
-- unit files will be stored. This is mandatory.
--
-- Also please note that it's strongly advised to avoid using a folder which is in your
-- "normal" source tree : if you run premake twice, those unit files will be included
-- in the project, as normal files. This is taken care of if you don't change the
-- compilation unit folder between 2 premake runs, but there is still the possibility
-- of screwing things.
--
-- So, assuming in you script you have this :
--
--		files { "src/**" }
--
-- It's advised to use something like this for the compilation unit directory :
--
--		compilationunitdir "compilation_units"
--		-- don't do this : compilationunitdir "src/compilation_units" !
--
premake.api.register {
	name = "compilationunitdir",
	scope = "project",
	kind = "path"
}


--
-- register our custom option
--
newoption {
	trigger = "compilationunit",
	value = 8,
	description = "generate a project which uses N compilation units. Defaults to 8 units."
}


--
-- This method overrides premake.oven.bakeFIles method. We use it to create the compilation
-- unit files and add them to the project.
--
function premake.extensions.compilationunit.customBakeFiles(base, prj)

	-- check that the folder in which to generate the compilation units is defined
	if prj.compilationunitdir == nil then
		return base(prj)
	end

	local project = premake.project
	local compilationunit = premake.extensions.compilationunit

	for cfg in project.eachconfig(prj) do

		-- init the list of compilation unit files
		local unitfiles = {}
		local unitfilenames = {}
		for i = 1, compilationunit.numcompilationunits do

			-- compute the filename
			unitfilenames[i] = cfg.compilationunitdir .. "/" .. cfg.architecture .. "/" .. cfg.buildcfg .. "/compilationunit" .. i .. ".cpp"

			-- open the compilation unit file
			unitfiles[i] = io.open(unitfilenames[i], "w")

			-- if we have a precompiled header, add it
			if cfg.pchheader ~= nil then
				unitfiles[i]:write("#include \"" .. cfg.pchheader .. "\"\n\n")
			end

		end

		-- iterate on the files to add them in the compilation units
		local index = 0
		local files = {}
		table.foreachi(cfg.files, function(filename)

			-- ignore old compilation units
			if compilationunit.isCompilationUnit(cfg, filename) == true then
				return
			end

			-- add the file to the list of "non-compilation unit" files
			table.insert(files, filename)

			-- ignore files which don't need to be included in the compilation unit ones
			-- (PCH source and includes)
			if compilationunit.isPCHSource(cfg, filename) == true or (path.iscfile(filename) == false and path.iscppfile(filename) == false)then
				return
			end

			-- compute the relative path of the original file, to add the #include statement
			-- in the compilation unit
			local relativefilename = path.getrelative(path.getdirectory(unitfilenames[index + 1]), path.getdirectory(filename))
			relativefilename = relativefilename .. "/" .. path.getname(filename)
			unitfiles[index + 1]:write("#include \"" .. relativefilename .. "\"\n")

			-- switch compilation unit, to try and keep an even load for each compilation unit.
			index = (index + 1) % compilationunit.numcompilationunits

		end)

		-- clear the list of files, and re-add the non-compilation units (this list
		-- doesn't contain any previously generated compilation unit files)
		cfg.files = files

		-- then add the compilation units
		table.foreachi(unitfilenames, function(filename)
			table.insert(cfg.files, filename)
		end)

		-- don't forget to close the files
		table.foreachi(unitfiles, function(file)
			file:close()
		end)

	end

	return base(prj)

end


--
-- This method overrides premake.fileconfig.addconfig and adds a file configuration object
-- for each file, on each configuration. We use it to disable compilation of non-compilation
-- units files.
--
function premake.extensions.compilationunit.customAddFileConfig(base, fcfg, cfg)

	-- call the base method to add the file config
	base(fcfg, cfg)

	-- do nothing else if the compilation units are not enabled for this project
	if cfg.compilationunitdir == nil then
		return
	end

	-- get it (the file configuration object)
	local config = premake.fileconfig.getconfig(fcfg, cfg)

	-- disable compilation of c/cpp files which are not compilation units, and not the PCH source
	local compilationunit = premake.extensions.compilationunit
	if (path.iscfile(filename) == true or path.iscppfile(filename) == true) and compilationunit.isCompilationUnit(cfg, fcfg.abspath) == false and compilationunit.isPCHSource(cfg, fcfg.abspath) == false then
		config.flags.ExcludeFromBuild = true
	end

end


--
-- Checks if an absolute filename is a compilation unit. Note that this method is
-- based on the value of `compilationunitdir`. If you change it, the next run won't
-- be able to detect the compilation units with this method. See the doc for the
-- API command `compilationunitdir` for the recommanded use.
--
-- @param cfg
--		The current configuration
-- @param absfilename
--		The absolute filename of the file to check
-- @return
-- 		true if the file is a compilation unit, false otherwise
--
function premake.extensions.compilationunit.isCompilationUnit(cfg, absfilename)
	return string.sub(absfilename, 1, string.len(cfg.compilationunitdir)) == cfg.compilationunitdir
end


--
-- Checks if a file is the PCH source.
--
-- @param cfg
--		The current configuration
-- @param absfilename
--		The absolute filename of the file to check
-- @return
-- 		true if the file is the PCH source, false otherwise
--
function premake.extensions.compilationunit.isPCHSource(cfg, absfilename)
	return cfg.pchsource ~= nil and cfg.pchsource:lower() == absfilename:lower()
end


--
-- If the compilationunit option was used, activate the addon
--
if _OPTIONS["compilationunit"] ~= nil then

	local compilationunit = premake.extensions.compilationunit

	-- store the number of compilation units
	compilationunit.numcompilationunits = tonumber(_OPTIONS["compilationunit"])
	if compilationunit.numcompilationunits == nil then
		error("value for option 'compilationunit' must be a valid number")
	end

	-- setup the overrides
	premake.override(premake.oven, "bakeFiles", compilationunit.customBakeFiles)
	premake.override(premake.fileconfig, "addconfig",  compilationunit.customAddFileConfig)

end
