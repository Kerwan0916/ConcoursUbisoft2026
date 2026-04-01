/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Audiokinetic Wwise generated include file. Do not edit.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __WWISE_IDS_H__
#define __WWISE_IDS_H__

#include <AK/SoundEngine/Common/AkTypes.h>

namespace AK
{
    namespace EVENTS
    {
        static const AkUniqueID PLAY_ALIEN_FS_RUN_SWITCH = 1484426712U;
        static const AkUniqueID PLAY_AMB_2D_ROOMTONE_GENERAL_LOOP = 73382271U;
        static const AkUniqueID PLAY_AMB_3D_HANGAR_DOOR_HUM_LOOP = 2548916635U;
        static const AkUniqueID PLAY_AMB_3D_INCUBATORS_LOOP = 1745445364U;
        static const AkUniqueID PLAY_AMB_3D_LEVERS_OFF_LOOP = 3110399011U;
        static const AkUniqueID PLAY_BGM = 3126765036U;
        static const AkUniqueID PLAY_DEBUG = 921702379U;
        static const AkUniqueID PLAY_ROOM_DOOR_ROOM_OPEN = 4182414899U;
        static const AkUniqueID PLAY_SFX_ALIEN_GRUNT_CATCHED = 2138504219U;
        static const AkUniqueID PLAY_SFX_ALIEN_GRUNT_SPOTTED = 1088840730U;
        static const AkUniqueID PLAY_SFX_ALIEN_GRUNT_VICTORY = 3279092899U;
        static const AkUniqueID PLAY_SFX_ALIEN_TELEPORTED = 2739294108U;
        static const AkUniqueID PLAY_SFX_BACK = 1780698603U;
        static const AkUniqueID PLAY_SFX_BUTTON_ACTIVATED = 1468578206U;
        static const AkUniqueID PLAY_SFX_BUTTON_APPEAR = 3301992102U;
        static const AkUniqueID PLAY_SFX_DOOR_HANGAR_OPEN = 2126514987U;
        static const AkUniqueID PLAY_SFX_HOVERS = 4230093191U;
        static const AkUniqueID PLAY_SFX_INCUBATOR_DOOR_OPEN = 1884588609U;
        static const AkUniqueID PLAY_SFX_KEYS_EMITTER_LOOP = 2012969758U;
        static const AkUniqueID PLAY_SFX_KEYS_PICKUP = 1592706777U;
        static const AkUniqueID PLAY_SFX_LEVER_LIGHT = 412978243U;
        static const AkUniqueID PLAY_SFX_LEVER_TRAP = 1484984344U;
        static const AkUniqueID PLAY_SFX_MENBLACK_FS_WALK_SWITCH = 2383364376U;
        static const AkUniqueID PLAY_SFX_MENBLACK_GRUNT_CATCH_ALIEN = 2800585656U;
        static const AkUniqueID PLAY_SFX_MENBLACK_GRUNT_SPOTS_ALIEN = 1709427562U;
        static const AkUniqueID PLAY_SFX_MENBLACK_IS_CLOSE_LOOP = 3303699548U;
        static const AkUniqueID PLAY_SFX_MENU_MAIN_ENTER = 3430759498U;
        static const AkUniqueID PLAY_SFX_MENU_MAIN_EXIT = 1160267864U;
        static const AkUniqueID PLAY_SFX_PANEL_IN = 1450419186U;
        static const AkUniqueID PLAY_SFX_PANEL_OUT = 4110738637U;
        static const AkUniqueID PLAY_SFX_SELECT = 325086926U;
    } // namespace EVENTS

    namespace STATES
    {
        namespace BGM_STATES
        {
            static const AkUniqueID GROUP = 2248712270U;

            namespace STATE
            {
                static const AkUniqueID ALIEN_SPOTTED = 4188752148U;
                static const AkUniqueID GAME_OVER = 1432716332U;
                static const AkUniqueID LEVEL = 2782712965U;
                static const AkUniqueID MAIN_MENU = 2005704188U;
                static const AkUniqueID NONE = 748895195U;
                static const AkUniqueID STORYBOARD_END = 1966203490U;
                static const AkUniqueID STORYBOARD_START = 93434497U;
                static const AkUniqueID WIN = 979765101U;
            } // namespace STATE
        } // namespace BGM_STATES

        namespace PLAY_PAUSE
        {
            static const AkUniqueID GROUP = 4233560256U;

            namespace STATE
            {
                static const AkUniqueID NONE = 748895195U;
                static const AkUniqueID PAUSE = 3092587493U;
                static const AkUniqueID PLAYING = 1852808225U;
            } // namespace STATE
        } // namespace PLAY_PAUSE

    } // namespace STATES

    namespace SWITCHES
    {
        namespace SURFACE
        {
            static const AkUniqueID GROUP = 1834394558U;

            namespace SWITCH
            {
                static const AkUniqueID RUG = 712161697U;
                static const AkUniqueID TILE = 2637588553U;
            } // namespace SWITCH
        } // namespace SURFACE

    } // namespace SWITCHES

    namespace GAME_PARAMETERS
    {
        static const AkUniqueID RTPC_VOLUME_BGM = 3993832622U;
        static const AkUniqueID RTPC_VOLUME_MAIN = 924658319U;
        static const AkUniqueID RTPC_VOLUME_SFX = 4276639445U;
        static const AkUniqueID SS_AIR_FEAR = 1351367891U;
        static const AkUniqueID SS_AIR_FREEFALL = 3002758120U;
        static const AkUniqueID SS_AIR_FURY = 1029930033U;
        static const AkUniqueID SS_AIR_MONTH = 2648548617U;
        static const AkUniqueID SS_AIR_PRESENCE = 3847924954U;
        static const AkUniqueID SS_AIR_RPM = 822163944U;
        static const AkUniqueID SS_AIR_SIZE = 3074696722U;
        static const AkUniqueID SS_AIR_STORM = 3715662592U;
        static const AkUniqueID SS_AIR_TIMEOFDAY = 3203397129U;
        static const AkUniqueID SS_AIR_TURBULENCE = 4160247818U;
    } // namespace GAME_PARAMETERS

    namespace BUSSES
    {
        static const AkUniqueID ALIENS = 84587813U;
        static const AkUniqueID AMB = 1117531639U;
        static const AkUniqueID BGM = 412724365U;
        static const AkUniqueID MASTER_AUDIO_BUS = 3803692087U;
        static const AkUniqueID MEN_IN_BLACK = 315418005U;
        static const AkUniqueID UI = 1551306167U;
        static const AkUniqueID WORLD_OBJECTS = 2002649911U;
    } // namespace BUSSES

    namespace AUDIO_DEVICES
    {
        static const AkUniqueID NO_OUTPUT = 2317455096U;
        static const AkUniqueID SYSTEM = 3859886410U;
    } // namespace AUDIO_DEVICES

}// namespace AK

#endif // __WWISE_IDS_H__
