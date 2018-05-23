/*****************************************************************************
 * wizard.cpp : wxWindows plugin for vlc
 *****************************************************************************
 * Copyright (C) 2000-2004 VideoLAN
 * $Id: wizard.cpp 7562 2004-04-30 09:14:47Z gbazin $
 *
 * Authors: Cl�ment Stenac <zorglub@videolan.org>
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
 * Preamble
 *****************************************************************************/
#include <stdlib.h>                                      /* malloc(), free() */
#include <errno.h>                                                 /* ENOMEM */
#include <string.h>                                            /* strerror() */
#include <stdio.h>

#include <vlc/vlc.h>
#include <vlc/intf.h>

#include "wxwindows.h"

#include <wx/statline.h>

class wizHelloPage;
class wizInputPage;

/*****************************************************************************
 * Define events
 *****************************************************************************/
enum
{
    ActionRadio0_Event, ActionRadio1_Event,
    MoreInfoStreaming_Event,
    MoreInfoTranscode_Event,

    Open_Event,Choose_Event,
    ListView_Event,
    InputRadio0_Event, InputRadio1_Event,
    PartialEnable_Event,

    MethodRadio0_Event, MethodRadio1_Event,
    MethodRadio2_Event, MethodRadio3_Event,

    EncapRadio0_Event, EncapRadio1_Event,
    EncapRadio2_Event, EncapRadio3_Event,
    EncapRadio4_Event, EncapRadio5_Event,
    EncapRadio6_Event, EncapRadio7_Event,
    EncapRadio8_Event, EncapRadio9_Event,

    VideoEnable_Event, VideoCodec_Event,
    AudioEnable_Event, AudioCodec_Event,

};

#define TEXTWIDTH 60
#define ACTION_STREAM 0
#define ACTION_TRANSCODE 1

#define MUX_PS          0
#define MUX_TS          1
#define MUX_MPEG        2
#define MUX_OGG         3
#define MUX_RAW         4
#define MUX_ASF         5
#define MUX_AVI         6
#define MUX_MP4         7
#define MUX_MOV         8

BEGIN_EVENT_TABLE(WizardDialog, wxWizard)
END_EVENT_TABLE()

/*****************************************************************************
 * Wizard strings
 *****************************************************************************/

#define ERROR_MSG _("Error")
#define ITEM_NAME _("Streaming/Transcoding Wizard")

/* Hello page */
#define HELLO_TITLE _("Streaming/Transcoding Wizard")
#define HELLO_TEXT _("This wizard helps you to stream, transcode or" \
                     " save a stream")
#define HELLO_STREAMING _("Stream")
#define HELLO_STREAMING_DESC _("Use this to stream on a network")
#define HELLO_TRANSCODE _("Transcode/Save")
#define HELLO_TRANSCODE_DESC _("Use this to re-encode a stream and save it to a file")
#define HELLO_NOTICE _("This wizard only gives access to a small subset of VLC's streaming and transcoding capabilities. Use the Open and Stream Output dialogs to get all of them")

#define MOREINFO_TRANSCODE _("Please notice that VLC is not very suited " \
             "for file to file transcoding. You should use its transcoding " \
             "features to save network streams, for example" )

/* Input page */
#define INPUT_TITLE _("Choose input")
#define INPUT_TEXT _("Choose here your input stream")

#define INPUT_OPEN _("Select a stream" )
#define INPUT_PL _( "Existing playlist item" )

#define CHOOSE_STREAM _("You must choose a stream")
#define NO_PLAYLIST _("Uh Oh! Unable to find playlist !")

#define PARTIAL _("Use this to read only a part of the stream. " \
                  "You must be able to control the incoming stream " \
                  "(for example, a file or a disc, but not an UDP " \
                  "network stream.")

#define INPUT_BUTTON _("Choose")

/* Transcode 1 */
#define TRANSCODE1_TITLE _("Transcode")
#define TRANSCODE1_TEXT _("If you want to change the compression format of the audio or video tracks, fill in this page. (If you only want to change the container format, proceed to next page)")

#define TR_VIDEO_TEXT0 _("If your stream has video and you want to " \
                         "transcode it, enable this")
#define TR_VIDEO_TEXT _("Select your video codec. Click one to get more " \
                        "information")

#define TR_AUDIO_TEXT0 _("If your stream has audio and you want to "\
                         "transcode it, enable this")
#define TR_AUDIO_TEXT _("Select your audio codec. Click one to get more " \
                        "information")

/* Streaming 1 */
#define STREAMING1_TITLE _("Streaming")
#define STREAMING1_TEXT _("In this page, you will select how your input stream will be sent.")


#define INVALID_MCAST_ADDRESS _("This does not appear to be a valid " \
                                "multicast address" )

/* Encap  */
#define ENCAP_TITLE _("Encapsulation format")
#define ENCAP_TEXT _("In this page, you will select how the stream will be "\
                     "encapsulated." )


/* Transcode 2 */
#define EXTRATRANSCODE_TITLE _("Additional transcode options")
#define EXTRATRANSCODE_TEXT _("In this page, you will define a few " \
                              "additionnal parameters for your transcoding" )

/* Streaming 2 */
#define EXTRASTREAMING_TITLE _("Additional streaming options")
#define EXTRASTREAMING_TEXT _("In this page, you will define a few " \
                              "additionnal parameters for your stream" )


/*****************************************************************************
 * Helper structures
 *****************************************************************************/
struct codec {
    char *psz_display;
    char *psz_codec;
    char *psz_descr;
    int muxers[9];
};

struct codec vcodecs_array[] =
{
    { "MPEG-1 Video" , "mp1v" , "MPEG-1 Video codec",
       {MUX_PS, MUX_TS, MUX_MPEG, MUX_OGG, MUX_AVI, MUX_RAW, -1,-1,-1 } },
    { "MPEG-2 Video" , "mp2v" , "MPEG-2 Video codec",
       {MUX_PS, MUX_TS, MUX_MPEG, MUX_OGG, MUX_AVI, MUX_RAW, -1,-1,-1 } },
    { "MPEG-4 Video" , "mp4v" , "MPEG-4 Video codec",
       {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_MP4,MUX_OGG,MUX_AVI,MUX_RAW, -1} },
    { "DIVX 1" ,"DIV1","Divx first version" ,
       {MUX_TS , MUX_MPEG , MUX_ASF , MUX_OGG , MUX_AVI , -1,-1,-1,-1 } },
    { "DIVX 2" ,"DIV2","Divx second version" ,
       {MUX_TS , MUX_MPEG , MUX_ASF , MUX_OGG , MUX_AVI , -1,-1,-1,-1 } },
    { "DIVX 3" ,"DIV3","Divx third version" ,
       {MUX_TS , MUX_MPEG , MUX_ASF , MUX_OGG , MUX_AVI , -1,-1,-1,-1 } },
    { "H 263" , "H263" , "H263 is ..." ,
       { MUX_TS, MUX_AVI, -1,-1,-1,-1,-1,-1,-1 } },
    { "I 263", "I263", "I263 is ..." ,
       { MUX_TS, MUX_AVI, -1,-1,-1,-1,-1,-1,-1 } },
    { "WMV 1" , "WMV1", "First version of WMV" ,
       {MUX_TS , MUX_MPEG , MUX_ASF , MUX_OGG , MUX_AVI , -1,-1,-1,-1 } },
    { "WMV 2" , "WMV2", "2 version of WMV" ,
       {MUX_TS , MUX_MPEG , MUX_ASF , MUX_OGG , MUX_AVI , -1,-1,-1,-1 } },
    { "MJPEG" , "MJPG", "MJPEG consists of a series of JPEG pictures" ,
       {MUX_TS , MUX_MPEG , MUX_ASF , MUX_OGG , MUX_AVI , -1,-1,-1,-1 } },
    { "Theora" , "theo", "Experimental free codec",
       {MUX_TS, -1,-1,-1,-1,-1,-1,-1,-1} },
    { "Dummy", "dummy", "Dummy codec (do not transcode)" ,
      {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_MP4,MUX_OGG,MUX_AVI,MUX_RAW,MUX_MOV}},
    { NULL,NULL,NULL , {-1,-1,-1,-1,-1,-1,-1,-1,-1}} /* Do not remove me */
};

struct codec acodecs_array[] =
{
    { "MPEG Audio" , "mpga" , "The standard MPEG audio (1/2) format" ,
       {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_OGG,MUX_AVI,MUX_RAW, -1,-1} },
    { "MP3" , "mp3" , "MPEG Audio Layer 3" ,
       {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_OGG,MUX_AVI,MUX_RAW, -1,-1} },
    { "MPEG 4 Audio" , "mp4a" , "Audio format for MPEG4" ,
       {MUX_TS, MUX_MP4, -1,-1,-1,-1,-1,-1,-1 } },
    { "A/52" , "a52" , "DVD audio format" ,
       {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_OGG,MUX_AVI,MUX_RAW, -1,-1} },
    { "Vorbis" , "vorb" , "This is a free audio codec" ,
       {MUX_OGG, -1,-1,-1,-1,-1,-1,-1,-1} },
    { "FLAC" , "flac" , "This is an audio codec" ,
       {MUX_OGG , MUX_RAW, -1,-1,-1,-1,-1,-1,-1} },
    { "Speex" , "spx" , "An audio codec dedicated to compression of voice" ,
       {MUX_OGG, -1,-1,-1,-1,-1,-1,-1,-1} },
    { "Dummy", "dummy", "Dummy codec (do not transcode)" ,
     {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_MP4,MUX_OGG,MUX_AVI,MUX_RAW,MUX_MOV}},
    { NULL,NULL,NULL , {-1,-1,-1,-1,-1,-1,-1,-1,-1}} /* Do not remove me */
};

struct method {
    char *psz_access;
    char *psz_method;
    char *psz_descr;
    char *psz_address;
    int   muxers[9];
};

struct method methods_array[] =
{
    {"udp:","UDP Unicast", "Use this to stream to a single computer",
     "Enter the address of the computer to stream to",
     { MUX_TS, -1,-1,-1,-1,-1,-1,-1,-1 } },
    {"udp:","UDP Multicast",
     "Use this to stream to a dynamic group of computers on a "
     "multicast-enabled network. This is the most efficient method "
     "to stream to several computers, but it does not work over Internet.",
     "Enter the multicast address to stream to in this field. "
     "This must be an IP address between 224.0.0.0 an 239.255.255.255 "
     "For a private use, enter an address beginning with 239.255.",
     { MUX_TS, -1,-1,-1,-1,-1,-1,-1,-1 } },
    {"http://","HTTP",
            "Use this to stream to several computers. This method is "
     "less efficient, as the server needs to send several times the "
     "stream.",
     "Enter the local addresses you want to listen to. Do not enter "
     "anything if you want to listen to all adresses or if you don't "
     "understand. This is generally the best thing to do. Other computers "
     "can then access the stream at http://yourip:8080 by default",
     { MUX_TS, MUX_PS, MUX_MPEG, MUX_OGG, MUX_RAW, MUX_ASF, -1,-1,-1} },
    { NULL, NULL,NULL,NULL , {-1,-1,-1,-1,-1,-1,-1,-1,-1}} /* Do not remove me */
};

struct encap {
    int   id;
    char *psz_mux;
    char *psz_encap;
    char *psz_descr;
};

struct encap encaps_array[] =
{
    { MUX_PS, "ps","MPEG PS", "MPEG Program Stream" },
    { MUX_TS, "ts","MPEG TS", "MPEG Transport Stream" },
    { MUX_MPEG, "ps", "MPEG 1", "MPEG 1 Format" },
    { MUX_OGG, "ogg," "OGG", "OGG" },
    { MUX_RAW, "raw", "RAW", "RAW" },
    { MUX_ASF, "asf","ASF", "ASF" },
    { MUX_AVI, "avi","AVI", "AVI" },
    { MUX_MP4, "mp4","MP4", "MPEG4" },
    { MUX_MOV, "mov","MOV", "MOV" },
    { -1 , NULL,NULL , NULL } /* Do not remove me */
};

/*****************************************************************************
 * All the pages of the wizard, declaration
 *****************************************************************************/

/* Declare classes */
class wizHelloPage : public wxWizardPageSimple
{
    public:
        wizHelloPage( wxWizard *parent);
        void OnActionChange(wxEvent& event);
        void OnWizardPageChanging(wxWizardEvent& event);
    protected:
        int i_action;
        WizardDialog *p_parent;
        wxRadioButton *action_radios[2];
        DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wizHelloPage, wxWizardPageSimple)
    EVT_RADIOBUTTON( ActionRadio0_Event, wizHelloPage::OnActionChange)
    EVT_RADIOBUTTON( ActionRadio1_Event, wizHelloPage::OnActionChange)
    EVT_WIZARD_PAGE_CHANGING(-1, wizHelloPage::OnWizardPageChanging)
END_EVENT_TABLE()


class wizInputPage : public wxWizardPage
{
    public:
        wizInputPage( wxWizard *, wxWizardPage *, intf_thread_t *);
        wizInputPage::~wizInputPage();
        void OnWizardPageChanging(wxWizardEvent& event);
        void OnInputChange(wxEvent& event);
        void OnEnablePartial(wxCommandEvent& event);
        virtual wxWizardPage *GetPrev() const;
        virtual wxWizardPage *GetNext() const;
        void SetStreamingPage( wxWizardPage *page);
        void SetTranscodePage( wxWizardPage *page);
        void SetAction( int i_action );
        void SetPintf( intf_thread_t *p_intf );
    protected:
        bool b_chosen;
        intf_thread_t *p_intf;
        int i_action;
        int i_input;

        void OnChoose( wxCommandEvent& event );

        WizardDialog *p_parent;
        wxRadioButton *input_radios[2];
        wxBoxSizer *mainSizer;
        wxArrayString mrl;
        wxTextCtrl *mrl_text;
        wxTextCtrl *from_text;
        wxTextCtrl *to_text;
        OpenDialog *p_open_dialog;
        wxListView *listview;
        wxPanel *open_panel;
        wxPanel *pl_panel;
        wxWizardPage *p_prev;
        wxWizardPage *p_streaming_page;
        wxWizardPage *p_transcode_page;
        DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wizInputPage, wxWizardPageSimple)
    EVT_RADIOBUTTON( InputRadio0_Event, wizInputPage::OnInputChange)
    EVT_RADIOBUTTON( InputRadio1_Event, wizInputPage::OnInputChange)
    EVT_BUTTON( Choose_Event, wizInputPage::OnChoose)
    EVT_CHECKBOX( PartialEnable_Event, wizInputPage::OnEnablePartial)
    EVT_WIZARD_PAGE_CHANGING(-1, wizInputPage::OnWizardPageChanging)
END_EVENT_TABLE()


class wizTranscodeCodecPage : public wxWizardPage
{
public:
    wizTranscodeCodecPage( wxWizard *parent, wxWizardPage *next);
    ~wizTranscodeCodecPage();
    void wizTranscodeCodecPage::OnWizardPageChanging(wxWizardEvent& event);
    virtual wxWizardPage *GetPrev() const;
    virtual wxWizardPage *GetNext() const;
    void SetPrev( wxWizardPage *page);
protected:
    wxCheckBox *video_checkbox;
    wxComboBox *video_combo;
    wxStaticText * video_text;
    wxCheckBox *audio_checkbox;
    wxComboBox *audio_combo;
    wxStaticText * audio_text;

    WizardDialog *p_parent;
    int i_audio_codec;
    int i_video_codec;

    char *vcodec;
    char *acodec;

    wxWizardPage *p_prev;
    wxWizardPage *p_next;

    void OnVideoCodecChange(wxCommandEvent& event);
    void OnAudioCodecChange(wxCommandEvent& event);
    void OnEnableVideo(wxCommandEvent& event);
    void OnEnableAudio(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wizTranscodeCodecPage, wxWizardPageSimple)
   EVT_CHECKBOX( VideoEnable_Event, wizTranscodeCodecPage::OnEnableVideo)
   EVT_CHECKBOX( AudioEnable_Event, wizTranscodeCodecPage::OnEnableAudio)
   EVT_COMBOBOX( VideoCodec_Event, wizTranscodeCodecPage::OnVideoCodecChange)
   EVT_COMBOBOX( AudioCodec_Event, wizTranscodeCodecPage::OnAudioCodecChange)
   EVT_WIZARD_PAGE_CHANGING(-1, wizTranscodeCodecPage::OnWizardPageChanging)
END_EVENT_TABLE()

class wizStreamingMethodPage : public wxWizardPage
{
public:
    wizStreamingMethodPage( wxWizard *parent, wxWizardPage *next);
    void OnWizardPageChanging(wxWizardEvent& event);
    virtual wxWizardPage *GetPrev() const;
    virtual wxWizardPage *GetNext() const;
    void SetPrev( wxWizardPage *page);
protected:
    DECLARE_EVENT_TABLE()
    int i_method;
    wxBoxSizer *mainSizer;
    wxStaticBoxSizer *address_sizer;
    wxStaticText *address_text;
    wxTextCtrl *address_txtctrl;
    WizardDialog * p_parent;
    void OnMethodChange(wxEvent& event);
    wxRadioButton *method_radios[4];
    wxWizardPage *p_prev;
    wxWizardPage *p_next;
};

BEGIN_EVENT_TABLE(wizStreamingMethodPage, wxWizardPageSimple)
    EVT_RADIOBUTTON( MethodRadio0_Event, wizStreamingMethodPage::OnMethodChange)
    EVT_RADIOBUTTON( MethodRadio1_Event, wizStreamingMethodPage::OnMethodChange)
    EVT_RADIOBUTTON( MethodRadio2_Event, wizStreamingMethodPage::OnMethodChange)
    EVT_RADIOBUTTON( MethodRadio3_Event, wizStreamingMethodPage::OnMethodChange)
    EVT_WIZARD_PAGE_CHANGING(-1, wizStreamingMethodPage::OnWizardPageChanging)
END_EVENT_TABLE()


class wizEncapPage : public wxWizardPage
{
public:
    wizEncapPage( wxWizard *parent);
    wizEncapPage::~wizEncapPage();
    void OnWizardPageChanging(wxWizardEvent& event);
    virtual wxWizardPage *GetPrev() const;
    virtual wxWizardPage *GetNext() const;
    void SetStreamingPage( wxWizardPage *page);
    void SetTranscodePage( wxWizardPage *page);
    void SetPrev( wxWizardPage *page);
    void SetAction( int );
    void EnableEncap( int encap );
protected:
    DECLARE_EVENT_TABLE()
    int i_encap;
    int i_mux;
    int i_action;
    void OnEncapChange(wxEvent& event);
    wxRadioButton *encap_radios[9];
    WizardDialog *p_parent;
    wxWizardPage *p_prev;
    wxWizardPage *p_streaming_page;
    wxWizardPage *p_transcode_page;
};

BEGIN_EVENT_TABLE(wizEncapPage, wxWizardPageSimple)
    EVT_WIZARD_PAGE_CHANGING(-1, wizEncapPage::OnWizardPageChanging)
    EVT_RADIOBUTTON( EncapRadio0_Event, wizEncapPage::OnEncapChange)
    EVT_RADIOBUTTON( EncapRadio1_Event, wizEncapPage::OnEncapChange)
    EVT_RADIOBUTTON( EncapRadio2_Event, wizEncapPage::OnEncapChange)
    EVT_RADIOBUTTON( EncapRadio3_Event, wizEncapPage::OnEncapChange)
    EVT_RADIOBUTTON( EncapRadio4_Event, wizEncapPage::OnEncapChange)
    EVT_RADIOBUTTON( EncapRadio5_Event, wizEncapPage::OnEncapChange)
    EVT_RADIOBUTTON( EncapRadio6_Event, wizEncapPage::OnEncapChange)
    EVT_RADIOBUTTON( EncapRadio7_Event, wizEncapPage::OnEncapChange)
    EVT_RADIOBUTTON( EncapRadio8_Event, wizEncapPage::OnEncapChange)
    EVT_RADIOBUTTON( EncapRadio9_Event, wizEncapPage::OnEncapChange)
END_EVENT_TABLE()

/* Additional settings for transcode */
class wizTranscodeExtraPage : public wxWizardPage
{
public:
    wizTranscodeExtraPage( wxWizard *parent, wxWizardPage *prev,
                            wxWizardPage *next);
    virtual wxWizardPage *GetPrev() const;
    virtual wxWizardPage *GetNext() const;
protected:
    DECLARE_EVENT_TABLE()
    void wizTranscodeExtraPage::OnSelectFile(wxCommandEvent&);
    WizardDialog *p_parent;
    wxWizardPage *p_prev;
    wxWizardPage *p_next;
};

BEGIN_EVENT_TABLE(wizTranscodeExtraPage, wxWizardPage)
    EVT_BUTTON( Open_Event, wizTranscodeExtraPage::OnSelectFile)
END_EVENT_TABLE()

/* Additional settings for streaming */
class wizStreamingExtraPage : public wxWizardPage
{
public:
    wizStreamingExtraPage( wxWizard *parent, wxWizardPage *prev,
                            wxWizardPage *next);
    virtual wxWizardPage *GetPrev() const;
    virtual wxWizardPage *GetNext() const;
protected:
    DECLARE_EVENT_TABLE()
    WizardDialog *p_parent;
    wxWizardPage *p_prev;
    wxWizardPage *p_next;
};

BEGIN_EVENT_TABLE(wizStreamingExtraPage, wxWizardPage)
END_EVENT_TABLE()


/* Local functions */
static int ismult( char *psz_uri );

/***************************************************************************
 * Implementation of the pages
 ***************************************************************************/


/***************************************************
 * First page: choose between stream and transcode *
 ***************************************************/
wizHelloPage::wizHelloPage( wxWizard *parent) : wxWizardPageSimple(parent)
{
        i_action = 0;
        p_parent = (WizardDialog *)parent;
        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

        /* Create the texts */
        mainSizer->Add( new wxStaticText(this, -1, wxU( HELLO_TITLE ) ),
                        0, wxALL, 5 );
        mainSizer->Add( new wxStaticText(this, -1,
                        wxU( vlc_wraptext(HELLO_TEXT , TEXTWIDTH, false ) ) ),
                        0, wxALL, 5 );

        /* Create the radio buttons with their helps */
        action_radios[0] = new wxRadioButton( this, ActionRadio0_Event,
                                              wxU( HELLO_STREAMING ) );
        action_radios[1] = new wxRadioButton( this, ActionRadio1_Event,
                                              wxU( HELLO_TRANSCODE ) );
        i_action = 0;

        wxFlexGridSizer *stream_sizer = new wxFlexGridSizer( 2,2,1 );

        stream_sizer->Add( action_radios[0], 0, wxALL, 5 );
        stream_sizer->Add( new wxButton( this, MoreInfoStreaming_Event,
                                wxU( _("More Info")) ), 0, wxALL |
                                wxEXPAND | wxALIGN_RIGHT, 5 );
        mainSizer->Add( stream_sizer, 0, wxALL| wxEXPAND, 5 );
        mainSizer->Add( new wxStaticText(this, -1,
                 wxU( vlc_wraptext( HELLO_STREAMING_DESC ,TEXTWIDTH, false))),
                        0, wxLEFT, 5 );

        wxBoxSizer *transcode_sizer = new wxBoxSizer( wxHORIZONTAL);

        transcode_sizer->Add( action_radios[1], 0, wxALL, 5 );
        transcode_sizer->Add( new wxButton( this, MoreInfoTranscode_Event,
                                wxU( _("More Info")) ), 0, wxALL |
                                wxALIGN_RIGHT, 5 );
        mainSizer->Add( transcode_sizer, 0, wxALL | wxEXPAND, 5 );
        mainSizer->Add( new wxStaticText(this, -1,
                 wxU( vlc_wraptext( HELLO_TRANSCODE_DESC ,TEXTWIDTH, false)))
                        , 0, wxBOTTOM, 5 );

        mainSizer->Add( new wxStaticLine(this, -1 ), 0, wxEXPAND| wxTOP|
                        wxBOTTOM, 5 );

        mainSizer->Add( new wxStaticText(this, -1,
                        wxU( vlc_wraptext(HELLO_NOTICE , TEXTWIDTH , false ))),
                        0, wxALL, 5 );

        SetSizer(mainSizer);
        mainSizer->Fit(this);
    }

void wizHelloPage::OnActionChange(wxEvent& event)
{
    i_action = event.GetId() - ActionRadio0_Event;
    ((wizInputPage *)GetNext())->SetAction( i_action );
    p_parent->SetAction( i_action );
}

void wizHelloPage::OnWizardPageChanging(wxWizardEvent& event)
{
    ((wizInputPage *)GetNext())->SetAction( i_action );
    p_parent->SetAction( i_action );
}

/************************************
 * Second page: choose input stream *
 ************************************/
wizInputPage::wizInputPage( wxWizard *parent, wxWizardPage *prev, intf_thread_t *_p_intf) :
              wxWizardPage(parent)
{
    p_prev = prev;
    p_intf = _p_intf;
    p_parent = (WizardDialog *)parent;
    b_chosen = false;
    p_open_dialog = NULL;
    mainSizer = new wxBoxSizer(wxVERTICAL);

    /* Create the texts */
    mainSizer->Add( new wxStaticText(this, -1, wxU( INPUT_TITLE ) ),
                    0, wxALL, 5 );
    mainSizer->Add( new wxStaticText(this, -1,
                    wxU( vlc_wraptext(INPUT_TEXT , TEXTWIDTH, false ) ) ),
                    0, wxALL, 5 );

    /* Create the radio buttons */
    input_radios[0] = new wxRadioButton( this, InputRadio0_Event ,
                               wxU( INPUT_OPEN ) );
    mainSizer->Add( input_radios[0], 0, wxALL, 5 );
    input_radios[1] = new wxRadioButton( this, InputRadio1_Event ,
                               wxU( INPUT_PL ) );
    i_input = 0;
    mainSizer->Add( input_radios[1], 0, wxALL, 5 );

    /* Open Panel */
    open_panel = new wxPanel(this, -1);
    open_panel->SetAutoLayout( TRUE );
    wxBoxSizer *openSizer = new wxBoxSizer(wxHORIZONTAL);

    mrl_text = new wxTextCtrl( open_panel, -1, wxU( "" ), wxDefaultPosition,
                              wxSize(200,25) );
    openSizer->Add( mrl_text, 0 , wxALL, 5 );
    openSizer->Add( new wxButton( open_panel, Choose_Event, wxU(_("Choose...")) ), 0, wxALL, 5 );
    open_panel->SetSizer( openSizer );
    openSizer->Layout();
    openSizer->Fit(open_panel);

    mainSizer->Add( open_panel );

    playlist_t *p_playlist = (playlist_t *)vlc_object_find( p_intf,
                                       VLC_OBJECT_PLAYLIST, FIND_ANYWHERE );

    if( p_playlist )
    {
        if( p_playlist->i_size > 0)
        {
            pl_panel = new wxPanel(this, -1);
            wxBoxSizer *plSizer = new wxBoxSizer( wxHORIZONTAL );
            listview = new wxListView( pl_panel, ListView_Event,
                                       wxDefaultPosition, wxSize(300,200),
                                       wxLC_REPORT | wxSUNKEN_BORDER );
            listview->InsertColumn( 0, wxU(_("Name")) );
            listview->InsertColumn( 1, wxU(_("URI")) );
            listview->SetColumnWidth( 0, 250 );
            listview->SetColumnWidth( 1, 50 );
            for( int i=0 ; i < p_playlist->i_size ; i++ )
            {
                wxString filename = wxL2U( p_playlist->pp_items[i]->input.
                                                                    psz_name );
                listview->InsertItem( i, filename );
                listview->SetItem( i, 1, wxL2U( p_playlist->pp_items[i]->
                                                            input.psz_uri) );
            }
            listview->Select( p_playlist->i_index , TRUE);
            plSizer->Add( listview, 1, wxALL | wxEXPAND , 5 );
            pl_panel->SetSizer( plSizer );
            plSizer->Layout();
            mainSizer->Add( pl_panel, 1, wxALL|wxEXPAND, 5 );
            pl_panel->Hide();
            mainSizer->Layout();
        }
        else
        {
            input_radios[1]->Disable();
        }
        vlc_object_release( p_playlist );
    }
    else
    {
        input_radios[1]->Disable();
    }

    /* Partial Extract Box */
    wxStaticBox *partial_box = new wxStaticBox( this, -1,
                    wxU(_("Partial Extract")) );

    wxStaticBoxSizer *partial_sizer = new wxStaticBoxSizer( partial_box,
                                                          wxVERTICAL );

    wxCheckBox *enable_checkbox = new wxCheckBox( this, PartialEnable_Event,
                                                wxU(_("Enable") ) );
    enable_checkbox->SetToolTip(wxU(_(PARTIAL) ) ) ;
    partial_sizer->Add( enable_checkbox, 0 , wxLEFT , 5 );

    wxFlexGridSizer *partial_sizer2 = new wxFlexGridSizer( 4,1,20 );
    partial_sizer2->Add( new wxStaticText(this, -1, wxU(_("From"))),0,wxLEFT ,5);
    from_text = new wxTextCtrl( this, -1, wxT(""),
                                  wxDefaultPosition, wxSize(80,25) );
    partial_sizer2->Add( from_text, 0 , wxALIGN_RIGHT);
    partial_sizer2->Add( new wxStaticText(this, -1, wxU(_("To"))),0,wxLEFT ,5);
    to_text = new wxTextCtrl( this, -1, wxT(""),
                                  wxDefaultPosition, wxSize(80,25) );
    partial_sizer2->Add( to_text, 0 , wxALIGN_RIGHT);

    partial_sizer->Add( partial_sizer2, 0, wxALL, 0 );

    mainSizer->Add( partial_sizer, 0, wxALL, 5 );

    from_text->Disable();
    to_text->Disable();
    SetSizer(mainSizer);
    mainSizer->Fit(this);
    mainSizer->Layout();
}

wizInputPage::~wizInputPage()
{
    if( p_open_dialog )
    {
            fprintf(stderr,"CA CRAINT, %p\n",p_open_dialog);
//        p_open_dialog->EndModal(wxID_CANCEL );
  //      delete p_open_dialog;
    }
}

void wizInputPage::OnInputChange(wxEvent& event)
{
    i_input = event.GetId() - InputRadio0_Event;
    if( i_input == 0 )
    {
        if( pl_panel )
        {
            pl_panel->Hide();
            mainSizer->Hide( pl_panel );
            open_panel->Show();
            mainSizer->Show( open_panel );
            mainSizer->Layout();
        }
    }
    else
    {
        open_panel->Hide();
        mainSizer->Hide( open_panel );
        pl_panel->Show();
        mainSizer->Show( pl_panel );
            mainSizer->Layout();
    }
}

void wizInputPage::OnEnablePartial(wxCommandEvent& event)
{
   from_text->Enable( event.IsChecked() );
   to_text->Enable( event.IsChecked() );
}


void wizInputPage::OnChoose(wxCommandEvent& event)
{
    p_open_dialog = new OpenDialog( p_intf, this, -1, -1, OPEN_STREAM );
    if( p_open_dialog->ShowModal() == wxID_OK )
    {
        mrl_text->SetValue(p_open_dialog->mrl[0] );
    }
    delete p_open_dialog;
    p_open_dialog = NULL;
}

void wizInputPage::OnWizardPageChanging(wxWizardEvent& event)
{
    if( i_input == 0)
    {
        if( mrl_text->GetValue().IsSameAs( wxT(""), TRUE ) &&
                        event.GetDirection() )
        {
            wxMessageBox( wxU( CHOOSE_STREAM ), wxU( ERROR_MSG ),
                          wxICON_WARNING | wxOK, this );
            event.Veto();
        }
        else
        {
            p_parent->SetMrl( (const char *)mrl_text->GetValue().c_str() );
            if( from_text->IsEnabled() )
            {
                msg_Dbg( p_intf, "Partial streaming enabled");
                int i_from = atoi( from_text->GetValue().mb_str() );
                int i_to = atoi( to_text->GetValue().mb_str() );
                p_parent->SetPartial( i_from, i_to );
            }
        }
    }
    else
    {
        int i = -1;
        wxListItem listitem;
        i = listview->GetNextItem( i , wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if( i != -1 )
        {
            listitem.SetId( i );
            listitem.SetColumn( 1 );
            listview->GetItem( listitem );
            p_parent->SetMrl( (const char*) listitem.GetText().c_str() );
        }
    }
    return;
}

wxWizardPage *wizInputPage::GetPrev() const { return p_prev; }
wxWizardPage *wizInputPage::GetNext() const
{
    if( i_action == ACTION_STREAM )
        return p_streaming_page;
    else
       return p_transcode_page;
}

void wizInputPage::SetStreamingPage( wxWizardPage *page)
{
    p_streaming_page = page;
}

void wizInputPage::SetTranscodePage( wxWizardPage *page)
{
    p_transcode_page = page;
}

void wizInputPage::SetAction( int i_action )
{
    this->i_action = i_action;
}

void wizInputPage::SetPintf( intf_thread_t *p_intf )
{
    this->p_intf = p_intf;
}

/***************************************************
 * First transcode page: choose codecs             *
 ***************************************************/
wizTranscodeCodecPage::wizTranscodeCodecPage( wxWizard *parent,
                       wxWizardPage *next) : wxWizardPage(parent)
{
    p_next = next;

    acodec = NULL;
    vcodec = NULL;
    p_parent = (WizardDialog *) parent;

    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);

    /* Add the main texts */
    main_sizer->Add( new wxStaticText(this, -1,
                wxU( vlc_wraptext( TRANSCODE1_TITLE ,TEXTWIDTH, false ) ) ),
                    0, wxALL, 5 );
    main_sizer->Add( new wxStaticText(this, -1,
                wxU( vlc_wraptext(TRANSCODE1_TEXT , TEXTWIDTH, false ) ) ),
                    0, wxALL, 5 );

    /* Video Box */
    wxStaticBox *video_box = new wxStaticBox( this, -1, wxU(_("Video")) );
    wxStaticBoxSizer *video_sizer = new wxStaticBoxSizer( video_box,
                                                          wxVERTICAL );
    /* Line 1 : only the checkbox */
    wxFlexGridSizer *video_sizer1 = new wxFlexGridSizer( 1,1,20 );
    video_sizer1->Add( new wxCheckBox( this, VideoEnable_Event,
                            wxU(_("Transcode video") ) ), 0 , wxLEFT , 5 );
    /* Line 2 : codec */
    wxFlexGridSizer *video_sizer2 = new wxFlexGridSizer( 2,1,20 );
    video_sizer2->Add( new wxStaticText(this, -1, wxU(_("Codec"))),0,wxLEFT ,5);
    video_combo = new wxComboBox( this, VideoCodec_Event, wxT(""),
                                  wxDefaultPosition, wxSize(200,25) );
    for( int i= 0; vcodecs_array[i].psz_display != NULL; i++ )
    {
        video_combo->Append( wxU( vcodecs_array[i].psz_display ) ,
                            (void *)&vcodecs_array[i] );
    }
    video_sizer2->Add( video_combo, 0 , wxALIGN_RIGHT);
    /* Line 3 : text */
    video_text = new wxStaticText( this, -1,
                     wxU( vlc_wraptext( TR_AUDIO_TEXT, TEXTWIDTH, false) ) );
    wxFlexGridSizer *video_sizer3 = new wxFlexGridSizer( 2,1,20 );
    video_sizer3->Add( video_text, 0, wxLEFT, 5 );
    /* Fill the main video sizer */
    video_sizer->Add( video_sizer1 , 0, wxEXPAND, 5 );
    video_sizer->Add( video_sizer2, 0, wxALL, 5 );
    video_sizer->Add( video_sizer3, 0, wxEXPAND );

    /* Audio box */
    wxStaticBox *audio_box = new wxStaticBox( this, -1, wxU(_("Audio")) );
    wxStaticBoxSizer *audio_sizer = new wxStaticBoxSizer( audio_box,
                                                          wxVERTICAL );
    /* Line1: enabler */
    wxFlexGridSizer *audio_sizer1 = new wxFlexGridSizer( 1,1,20);
    audio_sizer1->Add( new wxCheckBox( this, AudioEnable_Event,
                            wxU(_("Transcode audio") ) ), 0 , wxLEFT, 5 );
    /* Line 2 : codec */
    wxFlexGridSizer *audio_sizer2 = new wxFlexGridSizer( 2,1,20 );
    audio_sizer2->Add( new wxStaticText(this, -1, wxU(_("Codec"))),0,wxLEFT,5);
    audio_combo = new wxComboBox( this, AudioCodec_Event, wxT(""),
                                  wxDefaultPosition, wxSize(200,25) );
    for( int i= 0; acodecs_array[i].psz_display != NULL; i++ )
    {
        audio_combo->Append( wxU( acodecs_array[i].psz_display ) ,
                            (void *)&acodecs_array[i] );
    }
    audio_sizer2->Add( audio_combo, 0 , wxALIGN_RIGHT );
    /* Line 3 : text */
    audio_text = new wxStaticText( this, -1,
                     wxU( vlc_wraptext( TR_VIDEO_TEXT, TEXTWIDTH, false) ) );
    wxFlexGridSizer *audio_sizer3 = new wxFlexGridSizer( 2,1,20 );
    audio_sizer3->Add( audio_text, 0, wxLEFT,5 );

    audio_sizer->Add(audio_sizer1, 0, wxEXPAND, 5);
    audio_sizer->Add(audio_sizer2, 0, wxALL, 5 );
    audio_sizer->Add(audio_sizer3, 0, wxEXPAND );

    main_sizer->Add( video_sizer, 1, wxGROW, 0 );
    main_sizer->Add( audio_sizer, 1, wxGROW, 0 );
    main_sizer->Layout();
    SetSizerAndFit( main_sizer );

    /* Default disabled */
    video_combo->Disable(); video_text->Disable();
    audio_combo->Disable(); audio_text->Disable();

}

wizTranscodeCodecPage::~wizTranscodeCodecPage()
{
    if( acodec ) free( acodec );
    if( vcodec ) free( vcodec );
}

void wizTranscodeCodecPage::OnEnableVideo(wxCommandEvent& event)
{
   video_combo->Enable( event.IsChecked() );
   video_text->Enable( event.IsChecked() );
}

void wizTranscodeCodecPage::OnEnableAudio(wxCommandEvent& event)
{
   audio_combo->Enable( event.IsChecked() );
   audio_text->Enable( event.IsChecked() );
}

void wizTranscodeCodecPage::OnVideoCodecChange(wxCommandEvent& event)
{
    struct codec *c = (struct codec*)
             (video_combo->GetClientData(video_combo->GetSelection()));
    video_text->SetLabel( wxU( vlc_wraptext(c->psz_descr, TEXTWIDTH, false) ) );
    i_video_codec = video_combo->GetSelection();
    vcodec = strdup(c->psz_codec);
}

void wizTranscodeCodecPage::OnAudioCodecChange(wxCommandEvent& event)
{
    struct codec *c = (struct codec*)
             (audio_combo->GetClientData(audio_combo->GetSelection()));
    audio_text->SetLabel( wxU( vlc_wraptext(c->psz_descr, TEXTWIDTH, false) ) );
    i_audio_codec = audio_combo->GetSelection();
    acodec = strdup(c->psz_codec);

}

void wizTranscodeCodecPage::OnWizardPageChanging(wxWizardEvent& event)
{
    unsigned int i,j;

    if( !event.GetDirection() )
    {
            GetPrev()->Enable();
            return;
    }

    /* Set the dummy codec ( accept all muxers ) if needed */
    if( !video_combo->IsEnabled() )
    {
        i_video_codec = 12;
    }
    if( !audio_combo->IsEnabled() )
    {
        i_audio_codec = 7;
    }

    ((wizEncapPage *)GetNext())->SetPrev(this);

    for( i = 0 ; i< 9 ; i++ )
    {
        //fprintf(stderr,"vcodecs[%i].muxers[%i] = %i\n",i_video_codec,i,vcodecs_array[i_video_codec].muxers[i]);
        if( vcodecs_array[i_video_codec].muxers[i] != -1 )
        {
            for( j = 0 ; j< 9 ; j++ )
            {
//        fprintf(stderr,"  acodecs[%i].muxers[%i] = %i\n",i_audio_codec,j,acodecs_array[i_audio_codec].muxers[j]);
                if( acodecs_array[i_audio_codec].muxers[j] ==
                              vcodecs_array[i_video_codec].muxers[i] )
                {
                    ((wizEncapPage*)GetNext())->EnableEncap(
                               vcodecs_array[i_video_codec].muxers[i] );
                }
            }
        }
    }
    struct codec *c = (struct codec*)
             (video_combo->GetClientData( video_combo->IsEnabled() ?
                                          video_combo->GetSelection(): i_video_codec ));
    vcodec = strdup(c->psz_codec);
    c = (struct codec*)
           (audio_combo->GetClientData( audio_combo->IsEnabled() ?
                                       audio_combo->GetSelection() : i_audio_codec ));
    acodec = strdup(c->psz_codec);


    /* FIXME: Support bitrate */
    p_parent->SetTranscode( vcodec, 1000, acodec, 200 );
    ((wizEncapPage*)GetNext())->SetAction( p_parent->GetAction() );
    p_parent->SetAction( p_parent->GetAction() );

    return;
}

wxWizardPage *wizTranscodeCodecPage::GetPrev() const { return p_prev; }
wxWizardPage *wizTranscodeCodecPage::GetNext() const { return p_next; }
void wizTranscodeCodecPage::SetPrev( wxWizardPage *page) {p_prev = page; }


/***************************************************
 * First streaming page: choose method             *
 ***************************************************/
wizStreamingMethodPage::wizStreamingMethodPage( wxWizard *parent,
    wxWizardPage *next) : wxWizardPage(parent)
{
    int i;
    p_next = next;
    p_parent = (WizardDialog *)parent;

    mainSizer = new wxBoxSizer(wxVERTICAL);

    /* Create the texts */
    mainSizer->Add( new wxStaticText(this, -1, wxU(
                        vlc_wraptext( STREAMING1_TITLE, TEXTWIDTH, false) ) ),
                    0, wxALL, 5 );
    mainSizer->Add( new wxStaticText(this, -1,
                    wxU( vlc_wraptext(STREAMING1_TEXT , TEXTWIDTH, false ) ) ),
                    0, wxALL, 5 );

    i_method = 0;

    wxStaticBox *method_box = new wxStaticBox( this, -1,
                                               wxU(_("Streaming method")) );
    wxStaticBoxSizer *method_sizer = new wxStaticBoxSizer(method_box,
                                                          wxHORIZONTAL );
    for( i = 0 ; i< 3 ; i++ )
    {
        method_radios[i] = new wxRadioButton( this, MethodRadio0_Event + i,
                               wxU( methods_array[i].psz_method ) );
        method_radios[i]->SetToolTip( wxU(_( methods_array[i].psz_descr ) ) );
        method_sizer->Add( method_radios[i], 0, wxALL, 5 );
    }

    method_sizer->Layout();

    wxStaticBox *address_box = new wxStaticBox( this, -1,
                    wxU(_("Destination")) );

    address_sizer = new wxStaticBoxSizer(address_box,
                                         wxVERTICAL );

    address_text = new wxStaticText(this, -1,
                 wxU( vlc_wraptext( methods_array[2].psz_address,
                                    TEXTWIDTH, false ) ), wxDefaultPosition,
                  wxSize(200,25) );
    address_txtctrl = new wxTextCtrl( this, -1, wxU(""), wxDefaultPosition,
                                      wxSize(200,25));
    address_sizer->Add( address_text, 0, wxALL, 5 );
    address_sizer->Add( address_txtctrl, 0, wxALL, 5 );
    address_sizer->Layout();

    mainSizer->Add( method_sizer, 0, wxALL | wxEXPAND, 5 );
    mainSizer->Add( address_sizer, 0, wxALL | wxEXPAND, 5 );
    mainSizer->Layout();

    SetSizer(mainSizer);
    mainSizer->Fit(this);

    return;
}

void wizStreamingMethodPage::OnWizardPageChanging(wxWizardEvent& event)
{
    unsigned int i;
    if( !event.GetDirection() ) return;

    /* Check valid multicast address */
    if( i_method == 1 && !ismult((char *) address_txtctrl->GetValue().c_str()) )
    {
        wxMessageBox( wxU( INVALID_MCAST_ADDRESS ) , wxU( ERROR_MSG ),
                      wxICON_WARNING | wxOK, this );
        event.Veto();

    }
    ((wizEncapPage *)GetNext())->SetPrev(this);
    for( i = 0 ; i< 9 ; i++ )
    {
        if( methods_array[i_method].muxers[i] != -1 )
        {
            ((wizEncapPage*)GetNext())->EnableEncap(
                               methods_array[i_method].muxers[i] );
        }
    }
    p_parent->SetStream( methods_array[i_method].psz_access ,
                         (char *)address_txtctrl->GetValue().c_str() );
    return;
}

wxWizardPage *wizStreamingMethodPage::GetPrev() const { return p_prev; }
wxWizardPage *wizStreamingMethodPage::GetNext() const { return p_next; }

void wizStreamingMethodPage::SetPrev( wxWizardPage *page) {p_prev = page; }


void wizStreamingMethodPage::OnMethodChange(wxEvent& event)
{
    i_method = event.GetId() - MethodRadio0_Event;
    address_text->SetLabel( wxU(
     vlc_wraptext( _(methods_array[i_method].psz_address), TEXTWIDTH, false)));
    address_sizer->Layout();
    mainSizer->Layout();
}

/***************************************************
 * Choose encapsulation format                     *
 ***************************************************/
wizEncapPage::wizEncapPage( wxWizard *parent ) : wxWizardPage(parent)
{
    int i;
    i_mux = 0;
    p_parent = (WizardDialog *)parent;
    p_streaming_page = NULL;
    p_transcode_page = NULL;
    p_prev = NULL;
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    /* Create the texts */
    mainSizer->Add( new wxStaticText(this, -1, wxU(ENCAP_TITLE) ),
                    0, wxALL, 5 );
    mainSizer->Add( new wxStaticText(this, -1,
                    wxU( vlc_wraptext(ENCAP_TEXT , TEXTWIDTH, false ) ) ),
                    0, wxALL, 5 );

    for( i = 0 ; i< 9 ; i++ )
    {
        encap_radios[i] = new wxRadioButton( this, EncapRadio0_Event + i,
                               wxU( encaps_array[i].psz_encap ) );
        encap_radios[i]->SetToolTip( wxU(_( encaps_array[i].psz_descr ) ) );
        mainSizer->Add( encap_radios[i], 0, wxALL, 5 );
        encap_radios[i]->Disable();
    }

    SetSizer(mainSizer);
    mainSizer->Fit(this);
}

wizEncapPage::~wizEncapPage()
{
}

void wizEncapPage::OnWizardPageChanging(wxWizardEvent& event)
{
    int i;
    if( !event.GetDirection() )
    {
        for( i = 0 ; i< 9 ; i++ )
        {
            encap_radios[i]->Disable();
        }
    }
    p_parent->SetMux( encaps_array[i_mux].psz_mux );
    return;
}

void wizEncapPage::OnEncapChange(wxEvent& event)
{
    i_mux = event.GetId() - EncapRadio0_Event;
}

void wizEncapPage::EnableEncap( int encap )
{
    int i;
    for( i = 0 ; i< 9 ; i++)
    {
        if( encaps_array[i].id == encap )
        {
            encap_radios[i]->Enable();
            encap_radios[i]->SetValue(true);
            i_mux = i;
        }
    }
}

void wizEncapPage::SetStreamingPage( wxWizardPage *page)
{
    p_streaming_page = page;
}

void wizEncapPage::SetTranscodePage( wxWizardPage *page)
{
    p_transcode_page = page;
}

wxWizardPage *wizEncapPage::GetPrev() const { return p_prev; }

wxWizardPage *wizEncapPage::GetNext() const
{
       fprintf(stderr,"Action is %i (%i %i)",i_action,ACTION_STREAM,ACTION_TRANSCODE);
    if( i_action== ACTION_STREAM )
        return p_streaming_page;
    else
       return p_transcode_page;
}


void wizEncapPage::SetAction( int i_act  ) { i_action = i_act; }

void wizEncapPage::SetPrev( wxWizardPage *page) { p_prev = page; }

/***************************************************
 * Extra transcoding page : Select file            *
 ***************************************************/
wizTranscodeExtraPage::wizTranscodeExtraPage( wxWizard *parent,
                       wxWizardPage *prev,
                       wxWizardPage *next) : wxWizardPage(parent)
{
    p_next = next;
    p_prev = prev;
    p_parent = (WizardDialog *) parent;
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    /* Create the texts */
    mainSizer->Add( new wxStaticText(this, -1, wxU( EXTRATRANSCODE_TITLE )),
                    0, wxALL, 5 );
    mainSizer->Add( new wxStaticText(this, -1,
                   wxU( vlc_wraptext( EXTRATRANSCODE_TEXT , TEXTWIDTH,
                                       false ) ) ),  0, wxALL, 5 );
    mainSizer->Add( new wxButton( this, Open_Event, wxU("Open") ) );
    SetSizer(mainSizer);
    mainSizer->Fit(this);
}

void wizTranscodeExtraPage::OnSelectFile( wxCommandEvent &event)
{
    wxFileDialog *file_dialog =  new wxFileDialog( this, wxU(_("Open File")),
                   wxT(""), wxT(""), wxT("*"), wxSAVE );

    if( file_dialog && file_dialog->ShowModal() == wxID_OK )
    {
        if( file_dialog->GetFilename().mb_str() )
        {
            p_parent->SetTranscodeOut( (char*)file_dialog->GetFilename().
                                                  c_str() );
        }
    }

}

wxWizardPage *wizTranscodeExtraPage::GetPrev() const { return p_prev; }
wxWizardPage *wizTranscodeExtraPage::GetNext() const {return p_next; }

/***********************************************************
 *  Extra streaming page
 ***********************************************************/
wizStreamingExtraPage::wizStreamingExtraPage( wxWizard *parent,
                       wxWizardPage *prev,
                       wxWizardPage *next) : wxWizardPage(parent)
{
    p_next = next;
    p_prev = prev;
    p_parent = (WizardDialog *) parent;
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    /* Create the texts */
    mainSizer->Add( new wxStaticText(this, -1, wxU( EXTRASTREAMING_TITLE )),
                    0, wxALL, 5 );
    mainSizer->Add( new wxStaticText(this, -1,
                    wxU( vlc_wraptext(EXTRASTREAMING_TEXT , TEXTWIDTH,
                                       false ) ) ),  0, wxALL, 5 );
    SetSizer(mainSizer);
    mainSizer->Fit(this);
}

wxWizardPage *wizStreamingExtraPage::GetPrev() const { return p_prev; }
wxWizardPage *wizStreamingExtraPage::GetNext() const {return p_next; }


/***************************************************************************
 * Implementation of the wizard itself
 ***************************************************************************/
wizHelloPage *page1;
wizInputPage *page2 ;
wizTranscodeCodecPage *tr_page1 ;
wizStreamingMethodPage *st_page1;
wizTranscodeExtraPage *tr_page2 ;
wizStreamingExtraPage *st_page2;
wizEncapPage *encap_page;

WizardDialog::WizardDialog(intf_thread_t *_p_intf, wxWindow *_p_parent ) :
wxWizard( _p_parent, -1, wxU(_("Streaming/Transcoding Wizard")), wxNullBitmap, wxDefaultPosition)
{
    /* Initializations */
    p_intf = _p_intf;
    SetPageSize(wxSize(400,350));

    /* Initialize structure */
    i_action = 0;
    i_from = 0;
    i_to = 0;
    vb = 0;
    ab = 0;
    acodec=NULL;
    vcodec=NULL;

    page1 = new wizHelloPage(this);
    page2 = new wizInputPage(this, page1, p_intf);
    encap_page = new wizEncapPage(this );
    tr_page1 = new wizTranscodeCodecPage(this, encap_page );
    st_page1 = new wizStreamingMethodPage(this, encap_page);

    tr_page2 = new wizTranscodeExtraPage(this, encap_page, NULL );
    st_page2 = new wizStreamingExtraPage(this, encap_page, NULL );

    /* Page 1 -> 2 */
    page1->SetNext( page2 );
    /* 2->1 in constructor of 2 */

    /* Page 2 -> 3 */
    page2->SetTranscodePage(tr_page1);
    page2->SetStreamingPage(st_page1);
    page2->SetPintf( p_intf );
    tr_page1->SetPrev(page2);
    st_page1->SetPrev(page2);

    /* Page 3 -> 4 */
    encap_page->SetTranscodePage( tr_page2 );
    encap_page->SetStreamingPage( st_page2 );
    /* 3->4 in constructor of 3 */
//    encap_page->SetPrev(tr_page1);
}

WizardDialog::~WizardDialog()
{
    Destroy();
    delete page1;
    delete page2;
    delete tr_page1;
    delete st_page1 ;
    delete st_page2;
    delete tr_page2;
    delete encap_page;
}

void WizardDialog::SetMrl( const char *mrl )
{
    this->mrl = strdup( mrl );
}

void WizardDialog::SetPartial( int from, int to )
{
    msg_Dbg(p_intf, "From %i to %i", from, to );
    this->i_from = i_from;
    this->i_to = i_to;
}

void WizardDialog::SetTranscode( char *vcodec, int vb, char *acodec,int ab)
{
    if( strcmp( vcodec, "dummy") )
    {
        this->vcodec= strdup(vcodec);
    }
    if( strcmp( acodec, "dummy" ) )
    {
        this->acodec = strdup(acodec);
    }
    this->vb = vb;
    this->ab = ab;
}

void WizardDialog::SetStream( char *method, char *address )
{
    this->method = strdup( method );
    this->address = strdup( address );
}

void WizardDialog::SetTranscodeOut( char *address )
{
    this->address = strdup( address );
}

void WizardDialog::SetMux( char *mux )
{
    this->mux = strdup( mux );
}

void WizardDialog::SetAction( int i_action )
{
    this->i_action = i_action;
}

int WizardDialog::GetAction()
{
    return i_action;
}

void WizardDialog::Run()
{
    msg_Dbg( p_intf,"starting wizard");
    if( RunWizard(page1) )
    {
        int i_size;
        char *psz_opt;
        msg_Dbg( p_intf,"wizard completed");

        if( i_action == ACTION_TRANSCODE)
        {
            msg_Dbg( p_intf,"Starting transcode of %s to file %s",
                                  mrl, address);
            msg_Dbg( p_intf,"Using %s (%i kbps) / %s (%i kbps),encap %s",
                                vcodec,vb,acodec,ab,mux);
            int i_tr_size = 10; /* 10 = ab + vb */
            i_tr_size += vcodec ? strlen(vcodec) : 0;
            i_tr_size += acodec ? strlen(acodec) : 0;

            char *psz_transcode = (char *)malloc( i_tr_size * sizeof(char));
            if( vcodec || acodec )
            {
                sprintf( psz_transcode, "transcode{");
            }
            else
            {
                sprintf( psz_transcode, "%c", 0 );
            }
            if( vcodec )
            {
                sprintf( psz_transcode, "%svcodec=%s,vb=%i",
                                psz_transcode, vcodec, vb );
            }
            if( acodec )
            {
                sprintf( psz_transcode, "%s%cacodec=%s,ab=%i",
                                psz_transcode, vcodec ? ',' : ' ',
                                acodec, ab );
            }
            if( vcodec || acodec )
            {
                sprintf( psz_transcode, "%s}:", psz_transcode );
            }
            i_size = 73 + strlen(mux) + strlen(address) + strlen(psz_transcode);
            psz_opt = (char *)malloc( i_size * sizeof(char) );
            sprintf( psz_opt, ":sout=#%sstandard{mux=%s,url=%s,"
                               "access=file}",
                               psz_transcode, mux, address );
        }
        else
        {
            msg_Dbg( p_intf, "Starting stream of %s to %s using %s, encap %s",
                               mrl, address, method, mux);

            i_size = 40 + strlen(mux) + strlen(address);
            psz_opt = (char *)malloc( i_size * sizeof(char) );
            sprintf( psz_opt, ":sout=#standard{mux=%s,url=%s,access=%s}",
                            mux, address,method);
        }

        playlist_t *p_playlist = (playlist_t *)vlc_object_find( p_intf,
                            VLC_OBJECT_PLAYLIST, FIND_ANYWHERE);
        if( p_playlist )
        {
            playlist_item_t *p_item = playlist_ItemNew( p_playlist, mrl, ITEM_NAME );
            playlist_ItemAddOption( p_item, psz_opt);
            if( i_from != 0)
            {
                char psz_from[20];
                msg_Dbg( p_intf, "Setting starttime");
                sprintf( psz_from, "start-time=%i", i_from);
                playlist_ItemAddOption( p_item, psz_from);
            }
            if( i_to != 0)
            {
                char psz_to[20];
                sprintf( psz_to, "stop-time=%i", i_to);
                playlist_ItemAddOption( p_item, psz_to);
            }
            playlist_AddItem( p_playlist, p_item, PLAYLIST_GO, PLAYLIST_END );
            vlc_object_release(p_playlist);
        }
        else
        {
            wxMessageBox( wxU( NO_PLAYLIST ), wxU( ERROR_MSG ),
                          wxICON_WARNING | wxOK, this );
        }
    }
    else
    {
        msg_Dbg( p_intf, "wizard was cancelled");
    }
}
/****************************************************************
 * Local helper functions
 ****************************************************************/
static int ismult( char *psz_uri )
{
    char *psz_end;
    int  i_value;

    i_value = strtol( psz_uri, &psz_end, 0 );
    /* IPv6 */
    if( psz_uri[0] == '[')
    {
            if( strncasecmp( &psz_uri[1], "FF0" , 3) ||
                            strncasecmp( &psz_uri[2], "FF0" , 3))
                    return( VLC_TRUE );
            else
                    return( VLC_FALSE );
    }
    if( *psz_end != '.' ) { return( VLC_FALSE ); }

    return( i_value < 224 ? VLC_FALSE : VLC_TRUE );

}
