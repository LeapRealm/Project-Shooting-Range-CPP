using UnrealBuildTool;
using System.Collections.Generic;

public class ShootingRange_CPPTarget : TargetRules
{
	public ShootingRange_CPPTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "ShootingRange_CPP" } );
	}
}