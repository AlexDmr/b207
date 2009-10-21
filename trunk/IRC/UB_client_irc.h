
class UB_client_irc
{protected:
   CUB_ircClient		 ircClient;
   CUB_ircClientInfo	 clientInfo;
   CUB_ircClientMessage clientMessage;

   ubInt				iCommandStringPos = 0;
   ubBool				bActive           = ubTrue;

   ubChar				cResponseBuffer [CUB_IRC_MAX_MESSAGE];
   ubChar				cBuffer         [CUB_IRC_MAX_MESSAGE];
   ubChar				cCommandString  [CUB_IRC_MAX_MESSAGE];
   ubChar				cChannel        [CUB_IRC_MAX_CHANNEL];

 public:
   UB_client_irc()
    {sprintf (clientInfo.m_szNickName,    "AlexD_essai");
	sprintf (clientInfo.m_szAltNameName, "AlexD_essai2'");
	sprintf (clientInfo.m_szPassword,    "");
	sprintf (clientInfo.m_szRealName,    "ircUB - by United Bytes");

	// NEW: Ident server information
	sprintf (clientInfo.m_szIdentName,   "IrcubTest");
	clientInfo.m_bUseIdentServer = ubTrue;
	clientInfo.m_iIdentServerPort = 113;

	// NEW: Channel for chatting
	sprintf (cChannel, "#ircub");

	printf ("done.\n"
			"Initializing socket system ...");

	// Set the logfile 
	// Note: If ircUB has NO write rights for a log file,
	//       linux will report a simple 'segmentation fault'
	g_pLogFile->CreateLogFile ("ConsoleTestLog.txt");

	// Initialize socket system (this is needed on several platform, .e.g. Windows)
	ircClient.InitSocketSystem ();

	printf ("done.\n"
			"Connecting to IRC ...\n\n");

	// Connect to irc.quakenet.eu.org (62.79.27.110, port 6667)
	if (!ircClient.ConnectToIRC ("irc.quakenet.eu.org", 6667, &clientInfo))
	{
		// Something went wrong :(
		PrintError ();
	}
	else 
	{
		do {
			// This is your game loop, for example.
			MainLoop ();
			
			// In MessageManager () Ircub handles ALL messaging stuff,
			// just put it into your game loop!
			ircClient.MessageManager ();

		} while (bActive);
	}

	// Close Ircub
	ircClient.Shutdown ();

	// Shutdown socket system
	ircClient.ShutdownSocketSystem ();

};
