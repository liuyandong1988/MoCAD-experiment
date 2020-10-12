// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non - commercial, and by any
// means.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

using UnrealBuildTool;

public class RoadRunnerCarlaIntegration : ModuleRules
{
#if WITH_FORWARDED_MODULE_RULES_CTOR
	public RoadRunnerCarlaIntegration(ReadOnlyTargetRules Target) : base(Target) // 4.16 or later
#else
	public RoadRunnerCarlaIntegration(TargetInfo Target) // 4.15 or before
#endif
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Carla",
				"RoadRunnerImporter"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"XmlParser",
				"Carla",
				"RoadRunnerImporter"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

#if WITH_FORWARDED_MODULE_RULES_CTOR
		PublicDefinitions.Add("ASIO_NO_EXCEPTIONS");
		PublicDefinitions.Add("BOOST_NO_EXCEPTIONS");
		PublicDefinitions.Add("LIBCARLA_NO_EXCEPTIONS");
		PublicDefinitions.Add("PUGIXML_NO_EXCEPTIONS");
#else
		Definitions.Add("ASIO_NO_EXCEPTIONS");
		Definitions.Add("BOOST_NO_EXCEPTIONS");
		Definitions.Add("LIBCARLA_NO_EXCEPTIONS");
		Definitions.Add("PUGIXML_NO_EXCEPTIONS");
#endif
	}
}
