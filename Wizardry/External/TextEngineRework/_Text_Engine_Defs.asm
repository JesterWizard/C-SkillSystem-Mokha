.equ gpDialogueState, 						0x859133C
.equ gDialogueText, 						0x30000D0
.equ gBg0MapBuffer, 						0x2022CA8
.equ gProc_TextScrollInBoxBy2, 				0x8591470
.equ gProc_DialoguePauseTimer, 				0x85913D0
.equ gpCurrentFont,							0x2028E70
.equ gpDialoguePositionTable,				0x800895C	@part of GetDialogueFaceSlotXTile
.equ gMPlayTable,							0x8224404
.equ gTextChoiceDef_YesNo,					0x8591490
.equ gTextChoiceDef_Shop,					0x85914A0

.equ CheckDialogueFlag, 					0x8006ADC
.equ SetDialogueFlag, 						0x8006AA8
.equ UnsetDialogueFlag, 					0x8006AC8
.equ ResetNarrationLines, 					0x800815C
.equ ResetDialogueLines, 					0x8008250
.equ StartProc,								0x8002C7C
.equ StartBlockingProc, 					0x8002CE0
.equ _modsi3, 								0x80D1994
.equ Text_GetXCursor, 						0x8003E50
.equ NewTextBluArrowAndButtonChecker, 		0x8007CD8
.equ GetTextPauseDurationFromControlCode, 	0x8008198
.equ UnsetFaceDisplayBits38, 				0x80089C4
.equ Dialogue_SetActiveFacePosition, 		0x8007838
.equ LoadFace, 								0x8007854
.equ ShouldKeepTextBox, 					0x800890C
.equ ClearActiveTextBubble, 				0x80081A8
.equ StartFaceFade, 						0x8005F38
.equ StartBlockingTimer, 					0x8014238
.equ GetStringTextWidthWithDialogueCodes, 	0x8008B44
.equ Div, 									0x80D167C
.equ StartDialogueTextChoice, 				0x8007DE8
.equ BlockProc, 							0x8006EC4
.equ String_FromNumber, 					0x8014334
.equ MoveFaceFromr0tor1,					0x80079E4
.equ SetFaceBlinkControl, 					0x8006438
.equ SetFaceEyeControl, 					0x80064D4
.equ GetTacticianNameString,				0x80314E4
.equ ChangeTextColorID,						0x8006F00
.equ BXR1,									0x80D18C4
.equ CopyToPaletteBuffer,					0x8000DB8
.equ m4aSongNumStart,						0x80D01FC
.equ MPlayStart,							0x80D0930
.equ GetTextSpeed,							0x8030C94
.equ GetDialogueFaceSlotXTile,				0x8008934
.equ IsBattleDeamonActive,					0x804FDA0
.equ GetBgTileDataOffset, 					0x8000F8C
.equ Decompress,							0x8012F50
.equ BreakProcLoop,							0x8002E94
.equ Text_SetColorId,						0x8003E60
.equ GetStringTextWidth, 					0x8003EDC
.equ GetCharTextWidth,						0x8003F3C
.equ CreateMovingFaceProc,					0x8007A58
.equ SetFaceGfxConfigForBattle,				0x8007844

.equ MaxExtraTextCode, 						0x38
.equ FontIdOffset,							0x00
.equ TextColorGroupIdOffset,				0x01
.equ TextBoxBgPaletteIdOffset,				0x02
.equ TextBoxTypeIdOffset,					0x03
.equ TextBoopPitchIdOffset,					0x04
.equ TextAttributesOffsetInFaceProc,		0x4C
