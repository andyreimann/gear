Shader "Example/First Draft" {
    LocationBindings {
      inPosition ("Vertex Position", vec4) = { POSITION }
      inNormal ("Vertex Normal", vec3) = { NORMAL }
    }
    Propertie {
    	/* DEVELOPER HINTS

			Uniform locations for a shader are 
			cached at creation time!
    	*/

		diffuse ("Diffuse Color", vec4)
		ambient ("Ambient Color", vec4)
		specular ("Specular Color", vec4)
		shininess ("Shininess", float)
      	lightDir ("Direction of Light", vec3)
    }

    Shader {
      
      #VERTEXPROGRAM
        // include matrices and other default stuff
        #include "G2.defaults"

		out Data {
			vec3 normal;
			vec4 eye;
			vec4 color;
		} dataOut;

      	void main(void) {

			dataOut.normal = normalMatrix * inNormal;
			dataOut.eye = G2.modelViewMatrix * inPosition;
		    gl_Position = (G2.projectionMatrix * G2.modelViewMatrix) * inPosition;
		    dataOut.color = inColor;
		}
      
	  #FRAGMENTPROGRAM

	    out vec4 outColor; // here goes the final color

		in Data {
			vec3 normal;
			vec4 eye;
			vec4 color;
		} dataIn;

	  	void main() {
			
			// set the specular term to black
			vec4 spec = vec4(0.0);

			// normalize both input vectors
			vec3 n = normalize(dataIn.normal);
			vec3 e = normalize(vec3(dataIn.eye));
			float intensity = max(dot(n,light.dir), 0.0);
			
			vec4 color;
			#FOR NUM_LIGHTS FROM 0 TO 4 AS PERMUTATE FOR LIGHTS DO
				color =+ lightColors[#NUM_LIGHTS];
			#END
			// if the vertex is lit compute the specular color
			if (intensity > 0.0) {
				// compute the half vector
				vec3 h = normalize(light.dir + e);
				// compute the specular term into spec
				float intSpec = max(dot(h,n), 0.0);
				spec = material.specular * pow(intSpec,material.shininess);
			}
			
			outColor = max(intensity *  material.diffuse + spec, material.ambient);
			//outColor = vec4(n,1);
		}
      
    } 
    Fallback "Diffuse"
  }