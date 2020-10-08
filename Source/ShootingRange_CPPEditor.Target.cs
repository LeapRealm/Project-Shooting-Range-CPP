using UnrealBuildTool;
using System.Collections.Generic;

public class ShootingRange_CPPEditorTarget : TargetRules
{
	public ShootingRange_CPPEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "ShootingRange_CPP" } );
	}
}