/*****************************************************************************
 * modules_plugin.h : Plugin management functions used by the core application.
 *****************************************************************************
 * Copyright (C) 2001 VideoLAN
 * Automatically generated from ./src/misc/modules_plugin.h.in by bootstrap
 *
 * Authors: Samuel Hocevar <sam@zoy.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************/

/*****************************************************************************
 * STORE_SYMBOLS: store known symbols into p_symbols for plugin access.
 *****************************************************************************/
#define STORE_SYMBOLS( p_symbols ) \
    (p_symbols)->aout_FiltersCreatePipeline_inner = aout_FiltersCreatePipeline; \
    (p_symbols)->aout_FiltersDestroyPipeline_inner = aout_FiltersDestroyPipeline; \
    (p_symbols)->aout_FiltersPlay_inner = aout_FiltersPlay; \
    (p_symbols)->aout_OutputNextBuffer_inner = aout_OutputNextBuffer; \
    (p_symbols)->aout_FormatNbChannels_inner = aout_FormatNbChannels; \
    (p_symbols)->aout_FormatPrepare_inner = aout_FormatPrepare; \
    (p_symbols)->aout_FormatPrint_inner = aout_FormatPrint; \
    (p_symbols)->aout_FormatsPrint_inner = aout_FormatsPrint; \
    (p_symbols)->aout_FormatPrintChannels_inner = aout_FormatPrintChannels; \
    (p_symbols)->aout_FifoPop_inner = aout_FifoPop; \
    (p_symbols)->aout_FifoFirstDate_inner = aout_FifoFirstDate; \
    (p_symbols)->aout_VolumeSoftInit_inner = aout_VolumeSoftInit; \
    (p_symbols)->aout_VolumeNoneInit_inner = aout_VolumeNoneInit; \
    (p_symbols)->__aout_New_inner = __aout_New; \
    (p_symbols)->aout_Delete_inner = aout_Delete; \
    (p_symbols)->aout_DateInit_inner = aout_DateInit; \
    (p_symbols)->aout_DateSet_inner = aout_DateSet; \
    (p_symbols)->aout_DateMove_inner = aout_DateMove; \
    (p_symbols)->aout_DateGet_inner = aout_DateGet; \
    (p_symbols)->aout_DateIncrement_inner = aout_DateIncrement; \
    (p_symbols)->__aout_DecNew_inner = __aout_DecNew; \
    (p_symbols)->aout_DecDelete_inner = aout_DecDelete; \
    (p_symbols)->aout_DecNewBuffer_inner = aout_DecNewBuffer; \
    (p_symbols)->aout_DecDeleteBuffer_inner = aout_DecDeleteBuffer; \
    (p_symbols)->aout_DecPlay_inner = aout_DecPlay; \
    (p_symbols)->__aout_VolumeGet_inner = __aout_VolumeGet; \
    (p_symbols)->__aout_VolumeSet_inner = __aout_VolumeSet; \
    (p_symbols)->__aout_VolumeInfos_inner = __aout_VolumeInfos; \
    (p_symbols)->__aout_VolumeUp_inner = __aout_VolumeUp; \
    (p_symbols)->__aout_VolumeDown_inner = __aout_VolumeDown; \
    (p_symbols)->__aout_VolumeMute_inner = __aout_VolumeMute; \
    (p_symbols)->aout_Restart_inner = aout_Restart; \
    (p_symbols)->aout_FindAndRestart_inner = aout_FindAndRestart; \
    (p_symbols)->aout_ChannelsRestart_inner = aout_ChannelsRestart; \
    (p_symbols)->__config_GetType_inner = __config_GetType; \
    (p_symbols)->__config_GetInt_inner = __config_GetInt; \
    (p_symbols)->__config_PutInt_inner = __config_PutInt; \
    (p_symbols)->__config_GetFloat_inner = __config_GetFloat; \
    (p_symbols)->__config_PutFloat_inner = __config_PutFloat; \
    (p_symbols)->__config_GetPsz_inner = __config_GetPsz; \
    (p_symbols)->__config_PutPsz_inner = __config_PutPsz; \
    (p_symbols)->__config_LoadCmdLine_inner = __config_LoadCmdLine; \
    (p_symbols)->config_GetHomeDir_inner = config_GetHomeDir; \
    (p_symbols)->__config_LoadConfigFile_inner = __config_LoadConfigFile; \
    (p_symbols)->__config_SaveConfigFile_inner = __config_SaveConfigFile; \
    (p_symbols)->__config_ResetAll_inner = __config_ResetAll; \
    (p_symbols)->config_FindConfig_inner = config_FindConfig; \
    (p_symbols)->config_FindModule_inner = config_FindModule; \
    (p_symbols)->config_Duplicate_inner = config_Duplicate; \
    (p_symbols)->config_SetCallbacks_inner = config_SetCallbacks; \
    (p_symbols)->config_UnsetCallbacks_inner = config_UnsetCallbacks; \
    (p_symbols)->vlc_current_charset_inner = vlc_current_charset; \
    (p_symbols)->stream_ReadLine_inner = stream_ReadLine; \
    (p_symbols)->__stream_DemuxNew_inner = __stream_DemuxNew; \
    (p_symbols)->stream_DemuxSend_inner = stream_DemuxSend; \
    (p_symbols)->stream_DemuxDelete_inner = stream_DemuxDelete; \
    (p_symbols)->demux_vaControl_inner = demux_vaControl; \
    (p_symbols)->demux_Control_inner = demux_Control; \
    (p_symbols)->demux_vaControlDefault_inner = demux_vaControlDefault; \
    (p_symbols)->__demux2_New_inner = __demux2_New; \
    (p_symbols)->demux2_Delete_inner = demux2_Delete; \
    (p_symbols)->demux2_vaControlHelper_inner = demux2_vaControlHelper; \
    (p_symbols)->subtitles_Detect_inner = subtitles_Detect; \
    (p_symbols)->input_vaControl_inner = input_vaControl; \
    (p_symbols)->input_Control_inner = input_Control; \
    (p_symbols)->input_DeletePES_inner = input_DeletePES; \
    (p_symbols)->__input_CreateThread_inner = __input_CreateThread; \
    (p_symbols)->input_StopThread_inner = input_StopThread; \
    (p_symbols)->input_DestroyThread_inner = input_DestroyThread; \
    (p_symbols)->__input_SetStatus_inner = __input_SetStatus; \
    (p_symbols)->__input_SetRate_inner = __input_SetRate; \
    (p_symbols)->__input_Seek_inner = __input_Seek; \
    (p_symbols)->__input_Tell_inner = __input_Tell; \
    (p_symbols)->input_DumpStream_inner = input_DumpStream; \
    (p_symbols)->input_OffsetToTime_inner = input_OffsetToTime; \
    (p_symbols)->input_ToggleES_inner = input_ToggleES; \
    (p_symbols)->input_ChangeArea_inner = input_ChangeArea; \
    (p_symbols)->input_ChangeProgram_inner = input_ChangeProgram; \
    (p_symbols)->input_InitStream_inner = input_InitStream; \
    (p_symbols)->input_EndStream_inner = input_EndStream; \
    (p_symbols)->input_FindProgram_inner = input_FindProgram; \
    (p_symbols)->input_AddProgram_inner = input_AddProgram; \
    (p_symbols)->input_DelProgram_inner = input_DelProgram; \
    (p_symbols)->input_SetProgram_inner = input_SetProgram; \
    (p_symbols)->input_AddArea_inner = input_AddArea; \
    (p_symbols)->input_DelArea_inner = input_DelArea; \
    (p_symbols)->input_FindES_inner = input_FindES; \
    (p_symbols)->input_AddES_inner = input_AddES; \
    (p_symbols)->input_DelES_inner = input_DelES; \
    (p_symbols)->input_SelectES_inner = input_SelectES; \
    (p_symbols)->input_UnselectES_inner = input_UnselectES; \
    (p_symbols)->input_RunDecoder_inner = input_RunDecoder; \
    (p_symbols)->input_EndDecoder_inner = input_EndDecoder; \
    (p_symbols)->input_DecodePES_inner = input_DecodePES; \
    (p_symbols)->input_DecodeBlock_inner = input_DecodeBlock; \
    (p_symbols)->input_ClockManageControl_inner = input_ClockManageControl; \
    (p_symbols)->input_ClockManageRef_inner = input_ClockManageRef; \
    (p_symbols)->input_ClockGetTS_inner = input_ClockGetTS; \
    (p_symbols)->input_BuffersEnd_inner = input_BuffersEnd; \
    (p_symbols)->input_NewBuffer_inner = input_NewBuffer; \
    (p_symbols)->input_ReleaseBuffer_inner = input_ReleaseBuffer; \
    (p_symbols)->input_ShareBuffer_inner = input_ShareBuffer; \
    (p_symbols)->input_NewPacket_inner = input_NewPacket; \
    (p_symbols)->input_DeletePacket_inner = input_DeletePacket; \
    (p_symbols)->input_NewPES_inner = input_NewPES; \
    (p_symbols)->input_FillBuffer_inner = input_FillBuffer; \
    (p_symbols)->input_Peek_inner = input_Peek; \
    (p_symbols)->input_SplitBuffer_inner = input_SplitBuffer; \
    (p_symbols)->input_AccessInit_inner = input_AccessInit; \
    (p_symbols)->input_AccessReinit_inner = input_AccessReinit; \
    (p_symbols)->input_AccessEnd_inner = input_AccessEnd; \
    (p_symbols)->__intf_Eject_inner = __intf_Eject; \
    (p_symbols)->GetLang_1_inner = GetLang_1; \
    (p_symbols)->GetLang_2T_inner = GetLang_2T; \
    (p_symbols)->GetLang_2B_inner = GetLang_2B; \
    (p_symbols)->DecodeLanguage_inner = DecodeLanguage; \
    (p_symbols)->httpd_HostNew_inner = httpd_HostNew; \
    (p_symbols)->httpd_HostDelete_inner = httpd_HostDelete; \
    (p_symbols)->httpd_UrlNew_inner = httpd_UrlNew; \
    (p_symbols)->httpd_UrlNewUnique_inner = httpd_UrlNewUnique; \
    (p_symbols)->httpd_UrlCatch_inner = httpd_UrlCatch; \
    (p_symbols)->httpd_UrlDelete_inner = httpd_UrlDelete; \
    (p_symbols)->httpd_ClientModeStream_inner = httpd_ClientModeStream; \
    (p_symbols)->httpd_ClientModeBidir_inner = httpd_ClientModeBidir; \
    (p_symbols)->httpd_ClientIP_inner = httpd_ClientIP; \
    (p_symbols)->httpd_FileNew_inner = httpd_FileNew; \
    (p_symbols)->httpd_FileDelete_inner = httpd_FileDelete; \
    (p_symbols)->httpd_RedirectNew_inner = httpd_RedirectNew; \
    (p_symbols)->httpd_RedirectDelete_inner = httpd_RedirectDelete; \
    (p_symbols)->httpd_StreamNew_inner = httpd_StreamNew; \
    (p_symbols)->httpd_StreamDelete_inner = httpd_StreamDelete; \
    (p_symbols)->httpd_StreamHeader_inner = httpd_StreamHeader; \
    (p_symbols)->httpd_StreamSend_inner = httpd_StreamSend; \
    (p_symbols)->httpd_MsgInit_inner = httpd_MsgInit; \
    (p_symbols)->httpd_MsgAdd_inner = httpd_MsgAdd; \
    (p_symbols)->httpd_MsgGet_inner = httpd_MsgGet; \
    (p_symbols)->httpd_MsgClean_inner = httpd_MsgClean; \
    (p_symbols)->__module_Need_inner = __module_Need; \
    (p_symbols)->__module_Unneed_inner = __module_Unneed; \
    (p_symbols)->mstrtime_inner = mstrtime; \
    (p_symbols)->mdate_inner = mdate; \
    (p_symbols)->mwait_inner = mwait; \
    (p_symbols)->msleep_inner = msleep; \
    (p_symbols)->secstotimestr_inner = secstotimestr; \
    (p_symbols)->__net_OpenTCP_inner = __net_OpenTCP; \
    (p_symbols)->__net_OpenUDP_inner = __net_OpenUDP; \
    (p_symbols)->net_Close_inner = net_Close; \
    (p_symbols)->__net_Read_inner = __net_Read; \
    (p_symbols)->__net_ReadNonBlock_inner = __net_ReadNonBlock; \
    (p_symbols)->__net_Write_inner = __net_Write; \
    (p_symbols)->__net_Gets_inner = __net_Gets; \
    (p_symbols)->net_Printf_inner = net_Printf; \
    (p_symbols)->__sout_NewInstance_inner = __sout_NewInstance; \
    (p_symbols)->sout_DeleteInstance_inner = sout_DeleteInstance; \
    (p_symbols)->sout_InputNew_inner = sout_InputNew; \
    (p_symbols)->sout_InputDelete_inner = sout_InputDelete; \
    (p_symbols)->sout_InputSendBuffer_inner = sout_InputSendBuffer; \
    (p_symbols)->sout_AccessOutNew_inner = sout_AccessOutNew; \
    (p_symbols)->sout_AccessOutDelete_inner = sout_AccessOutDelete; \
    (p_symbols)->sout_AccessOutSeek_inner = sout_AccessOutSeek; \
    (p_symbols)->sout_AccessOutRead_inner = sout_AccessOutRead; \
    (p_symbols)->sout_AccessOutWrite_inner = sout_AccessOutWrite; \
    (p_symbols)->sout_MuxNew_inner = sout_MuxNew; \
    (p_symbols)->sout_MuxAddStream_inner = sout_MuxAddStream; \
    (p_symbols)->sout_MuxDeleteStream_inner = sout_MuxDeleteStream; \
    (p_symbols)->sout_MuxDelete_inner = sout_MuxDelete; \
    (p_symbols)->sout_MuxSendBuffer_inner = sout_MuxSendBuffer; \
    (p_symbols)->__sout_ParseCfg_inner = __sout_ParseCfg; \
    (p_symbols)->sout_cfg_parser_inner = sout_cfg_parser; \
    (p_symbols)->sout_stream_new_inner = sout_stream_new; \
    (p_symbols)->sout_stream_delete_inner = sout_stream_delete; \
    (p_symbols)->sout_AnnounceRegister_inner = sout_AnnounceRegister; \
    (p_symbols)->sout_AnnounceRegisterSDP_inner = sout_AnnounceRegisterSDP; \
    (p_symbols)->sout_AnnounceUnRegister_inner = sout_AnnounceUnRegister; \
    (p_symbols)->sout_AnnounceSessionCreate_inner = sout_AnnounceSessionCreate; \
    (p_symbols)->sout_AnnounceSessionDestroy_inner = sout_AnnounceSessionDestroy; \
    (p_symbols)->sout_AnnounceMethodCreate_inner = sout_AnnounceMethodCreate; \
    (p_symbols)->__var_Create_inner = __var_Create; \
    (p_symbols)->__var_Destroy_inner = __var_Destroy; \
    (p_symbols)->__var_Change_inner = __var_Change; \
    (p_symbols)->__var_Type_inner = __var_Type; \
    (p_symbols)->__var_Set_inner = __var_Set; \
    (p_symbols)->__var_Get_inner = __var_Get; \
    (p_symbols)->__var_AddCallback_inner = __var_AddCallback; \
    (p_symbols)->__var_DelCallback_inner = __var_DelCallback; \
    (p_symbols)->__vout_Request_inner = __vout_Request; \
    (p_symbols)->__vout_Create_inner = __vout_Create; \
    (p_symbols)->vout_Destroy_inner = vout_Destroy; \
    (p_symbols)->vout_VarCallback_inner = vout_VarCallback; \
    (p_symbols)->vout_ChromaCmp_inner = vout_ChromaCmp; \
    (p_symbols)->vout_CreatePicture_inner = vout_CreatePicture; \
    (p_symbols)->vout_InitFormat_inner = vout_InitFormat; \
    (p_symbols)->vout_InitPicture_inner = vout_InitPicture; \
    (p_symbols)->vout_AllocatePicture_inner = vout_AllocatePicture; \
    (p_symbols)->vout_DestroyPicture_inner = vout_DestroyPicture; \
    (p_symbols)->vout_DisplayPicture_inner = vout_DisplayPicture; \
    (p_symbols)->vout_DatePicture_inner = vout_DatePicture; \
    (p_symbols)->vout_LinkPicture_inner = vout_LinkPicture; \
    (p_symbols)->vout_UnlinkPicture_inner = vout_UnlinkPicture; \
    (p_symbols)->vout_PlacePicture_inner = vout_PlacePicture; \
    (p_symbols)->vout_vaControlDefault_inner = vout_vaControlDefault; \
    (p_symbols)->vout_RequestWindow_inner = vout_RequestWindow; \
    (p_symbols)->vout_ReleaseWindow_inner = vout_ReleaseWindow; \
    (p_symbols)->vout_ControlWindow_inner = vout_ControlWindow; \
    (p_symbols)->vout_CreateSubPicture_inner = vout_CreateSubPicture; \
    (p_symbols)->vout_DestroySubPicture_inner = vout_DestroySubPicture; \
    (p_symbols)->vout_DisplaySubPicture_inner = vout_DisplaySubPicture; \
    (p_symbols)->__block_New_inner = __block_New; \
    (p_symbols)->block_ChainAppend_inner = block_ChainAppend; \
    (p_symbols)->block_ChainLastAppend_inner = block_ChainLastAppend; \
    (p_symbols)->block_ChainRelease_inner = block_ChainRelease; \
    (p_symbols)->block_ChainExtract_inner = block_ChainExtract; \
    (p_symbols)->block_ChainGather_inner = block_ChainGather; \
    (p_symbols)->block_NewEmpty_inner = block_NewEmpty; \
    (p_symbols)->__block_FifoNew_inner = __block_FifoNew; \
    (p_symbols)->block_FifoRelease_inner = block_FifoRelease; \
    (p_symbols)->block_FifoEmpty_inner = block_FifoEmpty; \
    (p_symbols)->block_FifoPut_inner = block_FifoPut; \
    (p_symbols)->block_FifoGet_inner = block_FifoGet; \
    (p_symbols)->block_FifoShow_inner = block_FifoShow; \
        (p_symbols)->vlc_strdup_inner = vlc_strdup; \
        (p_symbols)->vlc_vasprintf_inner = vlc_vasprintf; \
        (p_symbols)->vlc_asprintf_inner = vlc_asprintf; \
            (p_symbols)->vlc_strndup_inner = vlc_strndup; \
        (p_symbols)->vlc_atof_inner = vlc_atof; \
        (p_symbols)->vlc_atoll_inner = vlc_atoll; \
        (p_symbols)->vlc_getenv_inner = vlc_getenv; \
            (p_symbols)->vlc_strcasecmp_inner = vlc_strcasecmp; \
            (p_symbols)->vlc_strncasecmp_inner = vlc_strncasecmp; \
        (p_symbols)->vlc_lseek_inner = vlc_lseek; \
    (p_symbols)->vlc_wraptext_inner = vlc_wraptext; \
    (p_symbols)->vlc_dgettext_inner = vlc_dgettext; \
    (p_symbols)->vlc_error_inner = vlc_error; \
    (p_symbols)->__intf_Create_inner = __intf_Create; \
    (p_symbols)->intf_RunThread_inner = intf_RunThread; \
    (p_symbols)->intf_StopThread_inner = intf_StopThread; \
    (p_symbols)->intf_Destroy_inner = intf_Destroy; \
    (p_symbols)->__msg_Generic_inner = __msg_Generic; \
    (p_symbols)->__msg_GenericVa_inner = __msg_GenericVa; \
    (p_symbols)->__msg_Info_inner = __msg_Info; \
    (p_symbols)->__msg_Err_inner = __msg_Err; \
    (p_symbols)->__msg_Warn_inner = __msg_Warn; \
    (p_symbols)->__msg_Dbg_inner = __msg_Dbg; \
    (p_symbols)->__msg_Subscribe_inner = __msg_Subscribe; \
    (p_symbols)->__msg_Unsubscribe_inner = __msg_Unsubscribe; \
    (p_symbols)->__vlm_New_inner = __vlm_New; \
    (p_symbols)->vlm_Delete_inner = vlm_Delete; \
    (p_symbols)->vlm_ExecuteCommand_inner = vlm_ExecuteCommand; \
    (p_symbols)->vlm_MessageDelete_inner = vlm_MessageDelete; \
    (p_symbols)->__vlc_object_create_inner = __vlc_object_create; \
    (p_symbols)->__vlc_object_destroy_inner = __vlc_object_destroy; \
    (p_symbols)->__vlc_object_attach_inner = __vlc_object_attach; \
    (p_symbols)->__vlc_object_detach_inner = __vlc_object_detach; \
    (p_symbols)->__vlc_object_get_inner = __vlc_object_get; \
    (p_symbols)->__vlc_object_find_inner = __vlc_object_find; \
    (p_symbols)->__vlc_object_yield_inner = __vlc_object_yield; \
    (p_symbols)->__vlc_object_release_inner = __vlc_object_release; \
    (p_symbols)->__vlc_list_find_inner = __vlc_list_find; \
    (p_symbols)->vlc_list_release_inner = vlc_list_release; \
    (p_symbols)->playlist_Command_inner = playlist_Command; \
    (p_symbols)->__playlist_ItemNew_inner = __playlist_ItemNew; \
    (p_symbols)->playlist_ItemDelete_inner = playlist_ItemDelete; \
    (p_symbols)->playlist_ItemAdd_inner = playlist_ItemAdd; \
    (p_symbols)->playlist_Add_inner = playlist_Add; \
    (p_symbols)->playlist_AddExt_inner = playlist_AddExt; \
    (p_symbols)->playlist_Clear_inner = playlist_Clear; \
    (p_symbols)->playlist_Delete_inner = playlist_Delete; \
    (p_symbols)->playlist_Disable_inner = playlist_Disable; \
    (p_symbols)->playlist_Enable_inner = playlist_Enable; \
    (p_symbols)->playlist_DisableGroup_inner = playlist_DisableGroup; \
    (p_symbols)->playlist_EnableGroup_inner = playlist_EnableGroup; \
    (p_symbols)->playlist_ItemSetGroup_inner = playlist_ItemSetGroup; \
    (p_symbols)->playlist_ItemSetName_inner = playlist_ItemSetName; \
    (p_symbols)->playlist_ItemSetDuration_inner = playlist_ItemSetDuration; \
    (p_symbols)->playlist_SetGroup_inner = playlist_SetGroup; \
    (p_symbols)->playlist_SetName_inner = playlist_SetName; \
    (p_symbols)->playlist_SetDuration_inner = playlist_SetDuration; \
    (p_symbols)->playlist_GetPositionById_inner = playlist_GetPositionById; \
    (p_symbols)->playlist_ItemGetById_inner = playlist_ItemGetById; \
    (p_symbols)->playlist_ItemGetByPos_inner = playlist_ItemGetByPos; \
    (p_symbols)->playlist_CreateGroup_inner = playlist_CreateGroup; \
    (p_symbols)->playlist_DeleteGroup_inner = playlist_DeleteGroup; \
    (p_symbols)->playlist_FindGroup_inner = playlist_FindGroup; \
    (p_symbols)->playlist_GroupToId_inner = playlist_GroupToId; \
    (p_symbols)->playlist_GetInfo_inner = playlist_GetInfo; \
    (p_symbols)->playlist_ItemGetInfo_inner = playlist_ItemGetInfo; \
    (p_symbols)->playlist_ItemGetCategory_inner = playlist_ItemGetCategory; \
    (p_symbols)->playlist_ItemCreateCategory_inner = playlist_ItemCreateCategory; \
    (p_symbols)->playlist_AddInfo_inner = playlist_AddInfo; \
    (p_symbols)->playlist_ItemAddInfo_inner = playlist_ItemAddInfo; \
    (p_symbols)->playlist_ItemAddOption_inner = playlist_ItemAddOption; \
    (p_symbols)->playlist_Sort_inner = playlist_Sort; \
    (p_symbols)->playlist_Move_inner = playlist_Move; \
    (p_symbols)->playlist_Import_inner = playlist_Import; \
    (p_symbols)->playlist_Export_inner = playlist_Export; \
    (p_symbols)->__vlc_threads_init_inner = __vlc_threads_init; \
    (p_symbols)->__vlc_threads_end_inner = __vlc_threads_end; \
    (p_symbols)->__vlc_mutex_init_inner = __vlc_mutex_init; \
    (p_symbols)->__vlc_mutex_destroy_inner = __vlc_mutex_destroy; \
    (p_symbols)->__vlc_cond_init_inner = __vlc_cond_init; \
    (p_symbols)->__vlc_cond_destroy_inner = __vlc_cond_destroy; \
    (p_symbols)->__vlc_thread_create_inner = __vlc_thread_create; \
    (p_symbols)->__vlc_thread_set_priority_inner = __vlc_thread_set_priority; \
    (p_symbols)->__vlc_thread_ready_inner = __vlc_thread_ready; \
    (p_symbols)->__vlc_thread_join_inner = __vlc_thread_join; \
    (p_symbols)->vout_AspectRatio_inner = vout_AspectRatio; \
    (p_symbols)->__vout_SynchroInit_inner = __vout_SynchroInit; \
    (p_symbols)->vout_SynchroRelease_inner = vout_SynchroRelease; \
    (p_symbols)->vout_SynchroReset_inner = vout_SynchroReset; \
    (p_symbols)->vout_SynchroChoose_inner = vout_SynchroChoose; \
    (p_symbols)->vout_SynchroTrash_inner = vout_SynchroTrash; \
    (p_symbols)->vout_SynchroDecode_inner = vout_SynchroDecode; \
    (p_symbols)->vout_SynchroEnd_inner = vout_SynchroEnd; \
    (p_symbols)->vout_SynchroDate_inner = vout_SynchroDate; \
    (p_symbols)->vout_SynchroNewPicture_inner = vout_SynchroNewPicture; \
    (p_symbols)->vout_ShowTextRelative_inner = vout_ShowTextRelative; \
    (p_symbols)->vout_ShowTextAbsolute_inner = vout_ShowTextAbsolute; \
    (p_symbols)->__vout_OSDMessage_inner = __vout_OSDMessage; \

