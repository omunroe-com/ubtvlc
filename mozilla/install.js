var version = "0.8.6";

initInstall( "VideoLAN", "VLC", version, 1);

var tmpFolder = getFolder( "Temporary" );

if ( ! fileExists( tmpFolder) )
{
	logComment( "Cannot find Temporary Folder!" );
	cancelInstall();
}


setPackageFolder( tmpFolder );

addFile( "http://downloads.videolan.org/pub/videolan/vlc/0.8.6/win32/vlc-0.8.6-win32.exe" );

var exe  = getFolder(tmpFolder, "vlc-0.8.6-win32.exe");
File.execute( exe );

performInstall();

