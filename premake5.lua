-- Irlan Robson
-- Bounce premake script
-- http://industriousone.com/premake

-- variable paths for the case files are moved
solution_name = "bounce"
working_dir = "."
solution_dir = "build/"
external_dir = "external/"
bounce_inc_dir = "include/"
bounce_src_dir = "src/"
examples_inc_dir = "examples/"
examples_src_dir = "examples/"
tests_inc_dir = "test/"
tests_src_dir = "test/"
obj_dir = "/obj/"
bin_dir = "/bin/"

-- or "" to make --help work
action = _ACTION or ""

-- premake main
solution (solution_name)
	location ( solution_dir .. "/" .. action )
	configurations { "debug", "release" }
	platforms { "x32", "x64" }
	
	-- note the use of "!" before objdir to force the specified path
	configuration "debug"
		targetdir ( solution_dir .. action .. bin_dir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}" )
		objdir ( "!" .. solution_dir .. action .. obj_dir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}" )
		defines { "_DEBUG" }
		symbols "On"
		rtti "Off"
		flags { "FloatFast" }
			
    configuration "release"
		targetdir ( solution_dir .. action .. bin_dir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}" )
		objdir ( "!" .. solution_dir .. action .. obj_dir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}" )
		defines { "NDEBUG" }
		optimize "On"
		rtti "Off"
		flags { "FloatFast" }

	configuration { "vs*" }		
		defines { "_CRT_SECURE_NO_WARNINGS" } 
	
	configuration { "windows" }
		defines { "_WIN32", "WIN32", "_WINDOWS" }

	filter "language:C++"
		buildoptions { "-std=c++11" }

	project "bounce"
		kind "StaticLib"
		language "C++"
		location ( solution_dir .. action )
		includedirs { bounce_inc_dir }
		vpaths { [""] = "bounce" }

		files 
		{ 
			bounce_inc_dir .. "/bounce/**.h", 
			bounce_inc_dir .. "/bounce/**.inl", 
			bounce_src_dir .. "/bounce/**.cpp" 
		}
	project "glad"
		kind "StaticLib"
		language "C"
		location ( solution_dir .. action )
		includedirs { external_dir }
		vpaths { ["Headers"] = "**.h", ["Sources"] = "**.c" }	

		files 
		{ 
			external_dir .. "/glad/khrplatform.h",
			external_dir .. "/glad/glad.h", 
			external_dir .. "/glad/glad.c"
		}

		configuration { "not windows", "not macosx" }
			files 
			{ 
				external_dir .. "/glad/glad_glx.h", 
				external_dir .. "/glad/glad_glx.c"
			}	
		 
	project "glfw"
		kind "StaticLib"
		language "C"
		location ( solution_dir .. action )
		includedirs { external_dir .. "/glfw" }
		defines { "_GLFW_USE_CONFIG_H" } -- see glfw_config.h
		vpaths { ["Headers"] = "**.h", ["Sources"] = "**.c" }

		-- files
		
		-- common
		files
		{
			external_dir .. "/glfw/glfw_config.h",
			external_dir .. "/glfw/glfw3.h",
			external_dir .. "/glfw/glfw3native.h",
			
			external_dir .. "/glfw/context.c", 		
			external_dir .. "/glfw/init.c",
			external_dir .. "/glfw/input.c",
			external_dir .. "/glfw/monitor.c",
			external_dir .. "/glfw/vulkan.c",
			external_dir .. "/glfw/window.c",
		}	
		
		-- windows
		configuration { "windows" }
			files 
			{	 
				external_dir .. "/glfw/win32_platform.h",
				external_dir .. "/glfw/win32_joystick.h",
				external_dir .. "/glfw/wgl_context.h",
				external_dir .. "/glfw/egl_context.h",
				
				external_dir .. "/glfw/win32_init.c",
				external_dir .. "/glfw/win32_joystick.c",
				external_dir .. "/glfw/win32_monitor.c",
				external_dir .. "/glfw/win32_time.c",
				external_dir .. "/glfw/win32_tls.c",
				external_dir .. "/glfw/win32_window.c",
				external_dir .. "/glfw/wgl_context.c",
				external_dir .. "/glfw/egl_context.c",				
			}

		-- linux
		configuration { "not windows", "not macosx" }
         	buildoptions { "-pthread" }
			files 
			{	 
				external_dir .. "/glfw/x11_platform.h",
				external_dir .. "/glfw/xkb_unicode.h",
				external_dir .. "/glfw/linux_joystick.h",
				external_dir .. "/glfw/posix_time.h",	
				external_dir .. "/glfw/posix_tls.h",		
				external_dir .. "/glfw/glx_context.h",
				external_dir .. "/glfw/egl_context.h",
				
				external_dir .. "/glfw/x11_init.c",	
				external_dir .. "/glfw/x11_monitor.c",
				external_dir .. "/glfw/x11_window.c",
				external_dir .. "/glfw/xkb_unicode.c",
				external_dir .. "/glfw/linux_joystick.c",
				external_dir .. "/glfw/posix_time.c",
				external_dir .. "/glfw/posix_tls.c",
				external_dir .. "/glfw/egl_context.c",
				external_dir .. "/glfw/glx_context.c",					
			}	
		 
	project "imgui"
		kind "StaticLib"
		language "C++"
		location ( solution_dir .. action )
		includedirs { external_dir } 
		vpaths { ["Headers"] = "**.h", ["Sources"] = "**.cpp" }		

		files 
		{ 
			external_dir .. "/imgui/**.h", 
			external_dir .. "/imgui/**.cpp" 
		}

	project "rapidjson"
		kind "StaticLib"
		language "C++"
		location ( solution_dir .. action )
		includedirs { external_dir } 
		vpaths { ["Headers"] = "**.h", ["Sources"] = "**.cpp" }		

		files 
		{ 
			external_dir .. "/rapidjson/**.h", 
			external_dir .. "/rapidjson/**.cpp" 
		}
	project "testbed"
		kind "ConsoleApp"
		language "C++"
		location ( solution_dir .. action )
		includedirs { external_dir, bounce_inc_dir, examples_inc_dir }
		vpaths { ["Headers"] = "**.h", ["Sources"] = "**.cpp" }

		files 
		{ 
			examples_inc_dir .. "/testbed/**.h", 
			examples_src_dir .. "/testbed/**.cpp" 
		}

		links { "glfw", "glad", "imgui", "bounce" }

		configuration { "windows" }
			links { "opengl32", "winmm" }
			
		configuration { "not windows", "not macosx" }
			links 
			{ 		
				"GL", "rt", "Xrandr", "Xinerama", "Xi", "Xcursor", 
				"m", "dl", "Xrender", "drm", "Xdamage", "X11-xcb", "xcb-glx", 
				"xcb-dri2", "xcb-dri3", "xcb-present", "xcb-sync", "xshmfence", 
				"Xxf86vm", "Xfixes", "Xext", "X11", "pthread", "xcb", "Xau", "Xdmcp" 
			}
		
	project "hello_world"
		kind "ConsoleApp"
		language "C++"
		location ( solution_dir .. action )
		includedirs { bounce_inc_dir, examples_inc_dir }
		vpaths { ["Headers"] = "**.h", ["Sources"] = "**.cpp" }

		files 
		{ 
			examples_inc_dir .. "/hello_world/**.h", 
			examples_src_dir .. "/hello_world/**.cpp" 
		}

		links { "bounce" }
-- build
if os.is "windows" then
	
    newaction
    {
        trigger = "solution_vs2015",
        description = "Generate solution",
        execute = function ()
	    os.execute ( "premake5 clean" )
            os.execute ( "premake5 vs2015" )
        end
    }

	newaction
    {
        trigger = "solution_vs2017",
        description = "Generate solution",
        execute = function ()
	    os.execute ( "premake5 clean" )
            os.execute ( "premake5 vs2017" )
        end
    }

    newaction
    {
        trigger = "doc",
        description = "Generate documentation",
        execute = function ()
	    os.execute ( "doxygen doxyfile" )
	    os.execute ( "start doc\\api\\html\\index.html" )
        end
    }
end		

-- clean
newaction
{
    trigger = "clean",
    description = "Clean solution",
    execute = function ()
          os.rmdir( "doc" )
	  os.rmdir( solution_dir )
    end
}
