// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace G2 
{

		class Logger 
		{
			public:
				Logger& operator<<(std::string const&  msg)
				{
					std::cout << msg;
					return *this;
				}

				Logger& operator<<(char const*  msg)
				{
					std::cout << msg;
					return *this;
				}
				
				template<typename T>
				Logger& operator<<(glm::detail::tvec2<T> const&  v)
				{
					std::cout << "[" << v.x << "," << v.y << "]";
					return *this;
				}
				
				template<typename T>
				Logger& operator<<(glm::detail::tvec3<T> const&  v)
				{
					std::cout << "[" << v.x << "," << v.y << "," << v.z << "]";
					return *this;
				}
				
				template<typename T>
				Logger& operator<<(glm::detail::tvec4<T> const&  v)
				{
					std::cout << "[" << v.x << "," << v.y << "," << v.z << "," << v.w << "]";
					return *this;
				}
				
				template<typename T>
				Logger& operator<<(glm::detail::tmat3x3<T> const&  m)
				{
					T const* v = glm::value_ptr(m);
					std::cout.precision(4);
					std::cout.flags(std::ios::right);
					
					std::cout.width(0);
					std::cout << "[";
					std::cout.width(6);
					std::cout << v[0];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[1];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[2];
					std::cout.width(0);
					std::cout << "]\n";
					
					std::cout.width(0);
					std::cout << "[";
					std::cout.width(6);
					std::cout << v[3];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[4];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[5];
					std::cout.width(0);
					std::cout << "]\n";
					
					std::cout.width(0);
					std::cout << "[";
					std::cout.width(6);
					std::cout << v[6];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[7];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[8];
					std::cout.width(0);
					std::cout << "]";

					std::cout.precision();
					std::cout.flags();
					return *this;
				}
				
				template<typename T>
				Logger& operator<<(glm::detail::tmat4x4<T> const&  m)
				{
					T const* v = glm::value_ptr(m);
					std::cout.precision(4);
					std::cout.flags(std::ios::right);
					
					std::cout.width(0);
					std::cout << "[";
					std::cout.width(6);
					std::cout << v[0];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[1];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[2];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[3];
					std::cout.width(0);
					std::cout << "]\n";

					
					std::cout.width(0);
					std::cout << "[";
					std::cout.width(6);
					std::cout << v[4];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[5];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[6];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[7];
					std::cout.width(0);
					std::cout << "]\n";

					
					std::cout.width(0);
					std::cout << "[";
					std::cout.width(6);
					std::cout << v[8];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[9];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[10];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[11];
					std::cout.width(0);
					std::cout << "]\n";

					std::cout.width(0);
					std::cout << "[";
					std::cout.width(6);
					std::cout << v[12];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[13];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[14];
					std::cout.width(0);
					std::cout << ",";
					std::cout.width(6);
					std::cout << v[15];
					std::cout.width(0);
					std::cout << "]";

					std::cout.precision();
					std::cout.flags();
					return *this;
				}

				template<typename T>
				Logger& operator<<(T  msg)
				{
					std::cout << msg;
					return *this;
				}
		};
		static Logger logger;
		static std::string endl = "\n";

	//Log& operator<<(std::string const&  msg);
};