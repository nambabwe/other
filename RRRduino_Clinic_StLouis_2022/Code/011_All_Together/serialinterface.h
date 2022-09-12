      u8IncomingByte = Serial.read( );
      switch( u8IncomingByte ) { 
        // volume down
        case '-':
          myDFPlayer.volumeDown( );                  // volume Down
          break;

        // volume up
        case '+':
          myDFPlayer.volumeUp( );                    // volume Up
          break;

        // 0
        case '0':
          playTrackNum( 0 );                         // should do nothing, just stop current
          printCurrentFileNum( false );
          break;

        // 1
        case '1':
          playTrackNum( 1 );                         // play 1
          printCurrentFileNum( false );
          break;

        // 2
        case '2':
          playTrackNum( 2 );                         // play 2
          printCurrentFileNum( false );
          break;

        // 3
        case '3':
          playTrackNum( 3 );                         // play 3
          printCurrentFileNum( false );
          break;

        // 4
        case '4':
          playTrackNum( 4 );                         // play 4
          printCurrentFileNum( false );
          break;

        // 5 
        case '5':
          playTrackNum( 5 );                         // play 5
          printCurrentFileNum( false );
          break;

        // play previous
        case 'P':
          myDFPlayer.stop( );                        // stop
          myDFPlayer.previous( );                    // play prior file
          Serial.print( F( "previous..." ) );
          printCurrentFileNum( false );
          break;

        // move to previous
        case 'p':
          myDFPlayer.stop( );                        // stop
          myDFPlayer.previous( );                    // play prior file
          myDFPlayer.stop( );                        // stop
          Serial.print( F( "previous..." ) );
          printCurrentFileNum( false );
          break;

        // play current
        case 'c':
          myDFPlayer.stop( );                        // stop
          myDFPlayer.next( );
          myDFPlayer.stop( );
          myDFPlayer.previous( );

          //playTrackX( (uint8_t)i16CurrentFile );     // play current file

          Serial.print( F( "current..." ) );
          printCurrentFileNum( false );
          break;

        // play next
        case 'N':
          myDFPlayer.stop( );                        // stop
          myDFPlayer.next( );                        // play next file
          Serial.print( F( "next..." ) );      
          printCurrentFileNum( false );
          break;  

        // move to next 
        case 'n':
          myDFPlayer.stop( );                        // stop
          myDFPlayer.next( );                        // play next file
          myDFPlayer.stop( );                        // stop
          Serial.print( F( "next..." ) );      
          printCurrentFileNum( false );
          break;

        // stop 
        case 's':
          myDFPlayer.stop( );                        // stop
          Serial.print( F( "stop..." ) );
          printCurrentFileNum( false );
          break;

        // start 
        case 'S':
          myDFPlayer.stop( );                        // stop
          myDFPlayer.start( );                       // start with the first file
          myDFPlayer.stop( );                        // stop
          Serial.print( F( "start..." ) );      
          printCurrentFileNum( false );
          break;

        // reset
        case 'R':
          myDFPlayer.stop( );                        // stop
          myDFPlayer.reset( );                       // reset
          Serial.print( F( "reset..." ) );
          printCurrentFileNum( false );
          break;

        // info
        case 'i':
          printPlayerInfo( );
          break;
          
        // menu
        case '?':
          printMenu( );
          break;

        // clear trigger
        case '!':
          myDistanceSensor.resetAll( );
          break;
          
        default:
          // unkown command
          break;
    } // switch u8IncomingByte
    
