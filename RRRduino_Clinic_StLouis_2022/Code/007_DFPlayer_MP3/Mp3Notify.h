
// Implement a notification class, its member methods will get called 

class Mp3Notify {
public:
  static void PrintlnSourceAction( DfMp3_PlaySources source, const char* action ) {
    if( source & DfMp3_PlaySources_Sd ) {
        Serial.print("SD Card, ");
    }
    if( source & DfMp3_PlaySources_Usb ) {
        Serial.print( F( "USB Disk, " ) );
    }
    if( source & DfMp3_PlaySources_Flash ) {
        Serial.print( F( "Flash, " ) );
    }
    Serial.println( action );
  } // static void PrintlnSourceAction( DfMp3_PlaySources, const char* ) {

  
  static void OnError( [[maybe_unused]] DfMp3& mp3, uint16_t errorCode ) {
    // see DfMp3_Error for code meaning
    Serial.println( );
    Serial.print( F( "Com Error " ) );
    Serial.println( errorCode );
  } // static void OnError( [[maybe_unused]] DfMp3&, uint16_t )

  
  static void OnPlayFinished( [[maybe_unused]] DfMp3& mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track ) {
    Serial.print( "Play finished for #" );
    Serial.println( track );  
  } // static void OnPlayFinished( [[maybe_unused]] DfMp3&, [[maybe_unused]] DfMp3_PlaySources, uint16_t ) {

  
  static void OnPlaySourceOnline( [[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source ) {
    PrintlnSourceAction( source, "online" );
  } // static void OnPlaySourceOnline( [[maybe_unused]] DfMp3&, DfMp3_PlaySources )

  
  static void OnPlaySourceInserted( [[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source ) {
    PrintlnSourceAction( source, "inserted" );
  } // static void OnPlaySourceInserted( [[maybe_unused]] DfMp3&, DfMp3_PlaySources ) {

  
  static void OnPlaySourceRemoved( [[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source ) {
    PrintlnSourceAction( source, "removed" );
  } // static void OnPlaySourceRemoved( [[maybe_unused]] DfMp3&, DfMp3_PlaySources )
};  // class Mp3Notify
