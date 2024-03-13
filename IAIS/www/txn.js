//
//

var MYNAME = '';
var arr = [];

//var MQTTIP = 'localhost'; var PORT = 9001; // websockets: '9001'; // native: '1883';
// use line below when standalone mqtt server at home
var MQTTIP = '10.10.10.13'; var PORT = 9001;  // websockets: '9001'; // native: '1883';

var WAKPUP = 5;

var SUBSCRIBED1 = '';
var SUBSCRIBED2 = '';
var SUBSCRIBED3 = '';

var delayInMilliseconds = 250; // 1/4 second

var DRAGGING = false;

var arr = [];
var imageList = [];
var texts = [];

imageList.push( { key: 0, closed: './images/left_e_c_short_wbut.gif',  thrown: './images/left_e_t_short_wbut.gif',  unknown: './images/left_e_u_short_wbut.gif' } );
imageList.push( { key: 1, closed: './images/right_w_c_short_wbut.gif', thrown: './images/right_w_t_short_wbut.gif', unknown: './images/right_w_u_short_wbut.gif' } );
imageList.push( { key: 2, closed: './images/line.gif',                 thrown: './images/line_occupied.gif',        unknown: './images/line.gif' } );
imageList.push( { key: 3, closed: './images/block_coupler_off.gif',    thrown: './images/block_coupler_on.gif',     unknown: './images/block_coupler_off.gif' } );
imageList.push( { key: 4, closed: './images/xover_c_thru.gif',         thrown: './images/xover_c_thru.gif',         unknown: './images/xover_c_thru.gif' } );
imageList.push( { key: 5, closed: './images/mainline.gif',             thrown: './images/mainline.gif',             unknown: './images/mainline.gif' } );
imageList.push( { key: 6, closed: './images/left_e_c_short.gif',       thrown: './images/left_e_t_short.gif',       unknown: './images/left_e_u_short.gif' } );
imageList.push( { key: 7, closed: './images/right_w_c_short.gif',      thrown: './images/right_w_t_short.gif',      unknown: './images/right_w_u_short.gif' } );
imageList.push( { key: 8, closed: './images/arrow_r.png',      thrown: './images/line.gif',      unknown: './images/line.gif' } );
imageList.push( { key: 9, closed: './images/arrow_l.png',      thrown: './images/line.gif',      unknown: './images/line.gif' } );
imageList.push( { key:10, closed: './images/left_e_c_short_wbut_dis.gif',  thrown: './images/left_e_t_short_wbut_dis.gif',  unknown: './images/left_e_u_short_wbut_dis.gif' } );
imageList.push( { key:11, closed: './images/right_w_c_short_wbut_dis.gif', thrown: './images/right_w_t_short_wbut_dis.gif', unknown: './images/right_w_u_short_wbut_dis.gif' } );
imageList.push( { key:12, closed: './images/yw_0d.gif',                thrown: './images/yo_0d.gif',                unknown: './images/yw_0d.gif' } );
imageList.push( { key:13, closed: './images/yw_1d.gif',                thrown: './images/yo_1d.gif',                unknown: './images/yw_0d.gif' } );
imageList.push( { key:14, closed: './images/yw_1b.gif',                thrown: './images/yo_1b.gif',                unknown: './images/yw_0d.gif' } );
imageList.push( { key:15, closed: './images/selectednot.gif',             thrown: './images/selected.gif',          unknown: './images/selectednot' } );
 
var logLine = ",,,";
var logSent = "...";

var curActive = 0;
      
Konva.showWarnings = true;

var textDebug = new Konva.Text( {
  x: 10, y: 390, fontFamily: 'Calibri', fontSize: 12, text: '   ', fill: 'black'
} );

function writeMessage( message ) {
  textDebug.setText( message );
  textLayer.draw( );
} // writeMessage( message )

var myWidth = window.innerWidth/1.1;
var myHeight = window.innerHeight/1.4;

function fitStageIntoParentContainer() {
  var container = document.querySelector('#stage-parent');
  
  // now we need to fit stage into parent
  var containerWidth = container.offsetWidth;
  // to do this we need to scale the stage
  var scale = containerWidth / myWidth;
  
  stage.width( myWidth * scale );
  stage.height( myHeight * scale );
  stage.scale( { x: scale, y: scale } );
  stage.draw( );
}

var tooltipLayer = new Konva.Layer( );

var textLayer = new Konva.Layer( );
var imageLayer = new Konva.Layer( );

var kImages = {};
var images = [];
var imageIDCs = imageList.length;

for( i = 0; i < imageIDCs; i++ ) {
  var imageObjClosed = new Image( );
  imageObjClosed.onload = function( ) {
    imageLayer.draw( );
  };
  imageObjClosed.src = imageList[i].closed;
  //if( DRAGGING )
  //  imageObjClosed.draggable = true;
    
  var imageObjThrown = new Image( );
  imageObjThrown.onload = function( ) {
    imageLayer.draw( );
  };

  imageObjThrown.src = imageList[i].thrown;
  //if( DRAGGING )
  //  imageObjThrown.draggable = true;     
  images.push( { key: i, closedImage: imageObjClosed, thrownImage: imageObjThrown } );
} // for

var connected = false;
var myIP; // = Math.floor( Math.random() * 100000 + 1 );
var client;// = new Paho.MQTT.Client( MQTTIP, 9001, MYNAME+"."+myIP );
var options = {
  onSuccess:onConnect,
  onFailure:doFail
} // options

//
//
//
function setupClient( myName ) {
  myIP = Math.floor( Math.random() * 100000 + 1 );

  client = new Paho.MQTT.Client( MQTTIP, PORT, myName+"."+myIP );
  client.onConnectionLost = onConnectionLost;
  client.onMessageArrived = onMessageArrived;
} // setupClient()

//
//
function loadImages( theImages, layer ) {
  var dX = 0;
  var dY = 0;
  for( var sorc in theImages ) {
    if( typeof ( theImages[sorc].offsetX ) !== typeof undefined ) {
      dX = theImages[sorc].offsetX;
      dY = theImages[sorc].offsetY;
      // remove one element, here: arr.splice( sorc,1 );
    } else {
      kImages[ sorc ] = new Konva.Image( {
        x: theImages[sorc].x+dX,
        y: theImages[sorc].y+dY,           
        width: theImages[sorc].width,
        height: theImages[sorc].height,
        rotation: theImages[sorc].rotation
      } );
      if( DRAGGING )
        kImages[ sorc ].draggable( true );
      kImages[ sorc ].imageIdx = theImages[ sorc ].imageIdx;
      kImages[ sorc ].id = theImages[ sorc ].key;
      kImages[ sorc ].ssn = theImages[ sorc ].ssn;
      if( typeof ( theImages[sorc].seq ) !== typeof undefined ) {
        kImages[ sorc ].seq = theImages[ sorc ].seq;
      }
      kImages[ sorc ].locked = theImages[ sorc ].locked;
      kImages[ sorc ].inv = false;
      if( theImages[ sorc ].inv )
        kImages[ sorc ].inv = true; 
      if( theImages[ sorc ].link )
        kImages[ sorc ].link = theImages[ sorc ].link;
      else
        kImages[ sorc ].link = "0";
      
      if( theImages[sorc].state ) {
        if( kImages[ sorc ].inv )
          kImages[ sorc ].setImage( images[ kImages[ sorc ].imageIdx].thrownImage );
        else
          kImages[ sorc ].setImage( images[ kImages[ sorc ].imageIdx].closedImage );
        kImages[ sorc ].state = true;           
      } else {
        if( kImages[ sorc ].inv )
          kImages[ sorc ].setImage( images[ kImages[ sorc ].imageIdx].closedImage );
        else
          kImages[ sorc ].setImage( images[ kImages[ sorc ].imageIdx].thrownImage );
        kImages[ sorc ].state = false;
      } // if

      if( DRAGGING ) {
        kImages[ sorc ].on( 'mouseover', dragged );
        kImages[ sorc ].on( 'dragmove', dragged );
        kImages[ sorc ].on( 'tap', dragged );
      } // if
      
      layer.add( kImages[ sorc ] );
      layer.draw( );
    } // else  
  } // for
} // function loadImages( theImages, layer )


// show the element's data when dragged
function dragged( e ) {
  if ( e ) {
    var lines = [
      'arr.push( { key: ' + e.target.id,
      'imageIdx: ' + e.target.imageIdx,
      'locked: ' + e.target.locked,
      'inv: ' + e.target.inv,
      'state: ' + e.target.state,
      'x: ' + e.target.x( ),
      'y: ' + e.target.y( ),
      'width: ' + e.target.width( ),
      'height: ' + e.target.height( ),
      'rotation: ' + e.target.rotation( ),
      'ssn: "' + e.target.ssn + '" } );'
    ];
    var newText = lines.join( '; ' );

    document.getElementById( '_info' ).innerHTML = "info ->" + newText;
    textLayer.batchDraw( );
  }
} // dragged( e )   


function isArray( a ) {
  return ( !!a ) && ( a.constructor === Array );
} // isArray( )

function doIt( evt ) {
  var tg = evt.target;
  var runTheSet = false;
  var activeOffUsed = false;

  if( tg.link != "0" ) {  
    console.log( "==>" + tg.link );
    window.open( tg.link, "_self" );
  } // if has link  

  if( connected ) {
    ///...writeMessage( "-->" + tg.id + ":s[" + tg.state + "]:L[" + tg.locked + "] " );
    if( tg.locked ) {
      console.log( "Locked-->" + tg.id + ": " + tg.locked + " " );
    } else {
      if( typeof ( tg.seq ) !== typeof undefined ) {
        if( tg.seq[0] == '!') {  
          activeOff( );
          activeOffUsed = true;
        } else {
          if( tg.seq[0] == ';') {
            console.log( "=>=>" + tg.id + " " + tg.seq );
            runTheSet = true;
            runThese = tg.seq;
          }

          if( curActive !== 0 ) {
            if( curActive.state ) {
              curActive.setImage( images[ curActive.imageIdx ].thrownImage );
              curActive.state = false;
              imageLayer.draw( );
            } else {
              curActive.setImage( images[ curActive.imageIdx ].closedImage );
              curActive.state = true;
              imageLayer.draw( );
            } // if  
      
            var state = curActive.state ? 'CLOSED' : 'THROWN';
            publish( curActive.ssn, state );
            writeMessage( "uu>" + curActive.id + ": " + curActive.state + " " + curActive.imageIdx + " " + curActive.ssn + " " + curActive.link + "\n" );
          }

          curActive = tg;
        } // if seq starts with ';'
      } // if has seq

      if( activeOffUsed ) {
        // do nothing
        } else {
        if( tg.state ) {
          tg.setImage( images[ tg.imageIdx ].thrownImage );
          tg.state = false;
          imageLayer.draw( );
        } else {
          tg.setImage( images[ tg.imageIdx ].closedImage );
          tg.state = true;
          imageLayer.draw( );
        } // if  

        var state = tg.state ? 'CLOSED' : 'THROWN';
        publish( tg.ssn, state );
        writeMessage( "==>" + tg.id + ": " + tg.state + " " + tg.imageIdx + " " + tg.ssn + " " + tg.link );
        
        // if( isArray( tg.ssn ) ) {
        //   tg.ssn.forEach( moveEach );
        //   runTheSet = true;
        //   runThese = tg.ssn;
        // } else {
        //   var state = tg.state ? 'CLOSED' : 'THROWN';
        //   publish( tg.ssn, state );
        // }
        ///...document.getElementById('_route').innerHTML = "out -->" + tg.ssn + " [" + state + "]";
      } // if locked
      console.log( "==>" + tg.id + ": " + tg.state + " " + tg.imageIdx + " " + tg.ssn + " " + tg.link );
      /* var rO = { errorCode: 1 };
        onConnectionLost( rO ); */  

      if( runTheSet ) {
        butClick( runThese );
      } // if a set
    } // if not activeUsed, like in normal day to day
  } else {
    document.getElementById( '_logLine' ).innerHTML = " Sorry, not connected, refresh the page please.";
    console.log( "!!>" + tg.id + "...not connected" );
  } // if connected
} // function doIt( evt )


// activeOff
function activeOff( ) {
  if( curActive == 0 ) return;

  if( curActive.state ) {
    curActive.setImage( images[ curActive.imageIdx ].thrownImage );
    curActive.state = false;
  } else {
    curActive.setImage( images[ curActive.imageIdx ].closedImage );
    curActive.state = true;
  } // if  
  imageLayer.draw( );

  var state = curActive.state ? 'CLOSED' : 'THROWN';
  publish( curActive.ssn, state );
  writeMessage( "uu>" + curActive.id + ": " + curActive.state + " " + curActive.imageIdx + " " + curActive.ssn + " " + curActive.link + "\n" );


  curActive = 0;
} // activeOff( )


function findSSNfromTG( theKey ) {
  console.log( "=>>" + kImages.length );
  for( var sorc in kImages ) {
    var theIm = kImages[ sorc ];
    console.log( "==>" + sorc ); 
    if( theIm.id === theKey ) {
      return theIm.ssn;
    } // if
  } // for
  return null;
} // findTG( key )


function moveEach( item ) {
  var theKey = Math.abs( item );
  var state = 'CLOSED';
  if( item < 0 ) {
    state = 'THROWN';
  } // if

  var TGssn = findSSNfromTG( theKey );
  if( TGssn == null ) {

  } else {
    publish( TGssn, state ); 
  }
} // moveEach( )


//
// called when the client connects
//
function onConnect( ) {
  console.log( "onConnect ("+MQTTIP+")" );

  if( SUBSCRIBED1.length > 0 ) {
    client.subscribe( SUBSCRIBED1+'/+' );
    console.log( "onConnect1: "+SUBSCRIBED1+"/+" );
  }
  if( SUBSCRIBED2.length > 0 ) {
    client.subscribe( SUBSCRIBED2+'/+' );
    console.log( "onConnect2: "+SUBSCRIBED2+"/+" );
  }
  if( SUBSCRIBED3.length > 0 ) {
    client.subscribe( SUBSCRIBED3+'/+' );
    console.log( "onConnect3: "+SUBSCRIBED3+"/+" );
  }
  connected = true;
  
  document.getElementById( '_logLine' ).innerHTML = "" + myIP + " connected to "+MQTTIP;
  document.getElementById( '_logLine' ).style.backgroundColor = "#3FFF3F";
} // onConnect( )


//
function doFail( e ){
  console.log( e );
} // doFail( )


//
function publish( topic, payload ) {
  if ( connected ) {
    message = new Paho.MQTT.Message( payload );
    message.destinationName = topic;
    message.retained = true;
    client.send( message );
  } // if( connected )
} // publish( )


// called when a message arrives
function onMessageArrived( message ) {
  var mask = 1;
  var topic = message.destinationName;
  var payload = message.payloadString;
  console.log( "onMessageArrived:" + topic + " [" + payload + "] " + topic[ topic.length-3] );
  if( topic[ topic.length-3] == '/' ) {    
  ///...document.getElementById( '_logSent' ).innerHTML = "in -->" + topic + " " + payload;
    for( var sorc in kImages ) { //arr ) {
      if( message.destinationName == kImages[ sorc ].ssn ) {
        if( ( message.payloadString == 'CLOSED' ) || ( message.payloadString == 'ACTIVE' ) ) {
          kImages[ sorc ].state = true;
          if( kImages[ sorc ].inv )
            kImages[ sorc ].setImage( images[ kImages[ sorc ].imageIdx].thrownImage );
	  else
            kImages[ sorc ].setImage( images[ kImages[ sorc ].imageIdx].closedImage );
        }
        if( ( message.payloadString == 'THROWN' ) || ( message.payloadString == 'INACTIVE' ) ){
          kImages[ sorc ].state = false;
          if( kImages[ sorc ].inv )
            kImages[ sorc ].setImage( images[ kImages[ sorc ].imageIdx].closedImage );
          else
            kImages[ sorc ].setImage( images[ kImages[ sorc ].imageIdx].thrownImage );
        }
        imageLayer.draw( );
        //break;
      } // if
    } // for
  } // if '/'
  else {
    console.log( "!" );
  } // if topic ends with /xx
} // onMessageArrived( )


// called when the client loses its connection
function onConnectionLost( responseObject ) {
  if ( responseObject.errorCode !== 0 ) {
    console.log( "onConnectionLost" ); //+ responseObject.errorMessage );
    connected = false;
    
    document.getElementById( '_logLine' ).innerHTML = "disconnected!";
    document.getElementById( '_logLine' ).style.backgroundColor = "#FF3F3F";
  } // if( )
} // onConnectionLost( )


//
document.addEventListener( 'DOMContentLoaded', ( ) => {
  'use strict';
  const charList = 'dr0123456789';
  
  console.log( 'content loaded' );

  document.addEventListener( 'keydown', event => {
    const key = event.key.toLowerCase( );

    // we are only interested in alphanumeric keys
    if( charList.indexOf( key ) === -1 )
      return;
    
//    console.log( key );
    if( key == 'd' ) {
      if( connected ) {
        // disconnect the client
        console.log( "disconnecting..." );
        client.disconnect( );
        var responseObject = { errorCode: 1 };
        onConnectionLost( responseObject );
      } else {
        console.log( "already disconnected!" );
      } // if connected
      return;
    } // disconnect
    if( key == 'r' ) {
      if( !connected ) {
        // connect the client
        console.log( "connecting..." );
        setupClient( MYNAME );

        options = {
          onSuccess:onConnect,
          onFailure:doFail
        } // options
        client.connect( options );
      } else {
        console.log( "already connected!" );
      } // if not connected
      return;
    } // disconnect    
  });
});

