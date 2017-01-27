/**
 * Background Subtraction (Golan Levin) + lauchpad virtuel (Patrick Saint-Denis)
 * 
 * Ajout d'un lauchpad virtuel à l'exemple de soustraction d'arrière-plan de G Levin. 
 *
 */


import processing.video.*;
import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;

int nbCol = 5;
int nbRow = 5;
int presenceThresh = 1500000;
int numPixels;
int[] backgroundPixels;
int[][] presenceSum = new int[nbCol][nbRow];
int[][] lastPresenceSum = new int[nbCol][nbRow];
Capture video;

void setup() {
  size(640, 480); 
  
  oscP5 = new OscP5(this,12000);
  myRemoteLocation = new NetAddress("127.0.0.1",13000);
  
  // This the default video input, see the GettingStartedCapture 
  // example if it creates an error
  //video = new Capture(this, 160, 120);
  video = new Capture(this, width, height);
  
  // Start capturing the images from the camera
  video.start();  
  
  numPixels = video.width * video.height;
  // Create array to store the background image
  backgroundPixels = new int[numPixels];
  // Make the pixels[] array available for direct manipulation
  loadPixels();
}

void draw() {
  if (video.available()) {
    video.read(); // Read a new video frame
    video.loadPixels(); // Make the pixels of video available
    
    //Une sommation en luminance pour chaque carré du launchpad est mise dans l'array presenceSum
    //qui est initialisée à chaque cadre
    for(int i=0; i<nbCol; i++) {
      for(int j=0; j<nbRow; j++) {
        presenceSum[i][j] = 0;
      }
    }
    
    // Difference between the current frame and the stored background
    for (int i = 0; i < numPixels; i++) { // For each pixel in the video frame...
      // Fetch the current color in that location, and also the color
      // of the background in that spot
      color currColor = video.pixels[i];
      color bkgdColor = backgroundPixels[i];
      // Extract the red, green, and blue components of the current pixel's color
      int currR = (currColor >> 16) & 0xFF;
      int currG = (currColor >> 8) & 0xFF;
      int currB = currColor & 0xFF;
      // Extract the red, green, and blue components of the background pixel's color
      int bkgdR = (bkgdColor >> 16) & 0xFF;
      int bkgdG = (bkgdColor >> 8) & 0xFF;
      int bkgdB = bkgdColor & 0xFF;
      // Compute the difference of the red, green, and blue values
      int diffR = abs(currR - bkgdR);
      int diffG = abs(currG - bkgdG);
      int diffB = abs(currB - bkgdB);
      
      // petite modification de l'exemple de Levin ici pour tenir compte de la
      // sous-division du canevas en carré de lauchpad
      presenceSum[int((i%width)/(width/nbCol))][int((i/width)/(height/nbCol))] += diffR + diffG + diffB;
      
      // Render the difference image to the screen
      pixels[i] = color(diffR, diffG, diffB);
      // The following line does the same thing much faster, but is more technical
      //pixels[i] = 0xFF000000 | (diffR << 16) | (diffG << 8) | diffB;
    }
    updatePixels(); // Notify that the pixels[] array has changed
    println(presenceSum[0][0]); // Print out the total amount of movement
    
    
  }

  //ici on dessine le lauchpad
  stroke(255);
  for(int i=0; i<nbCol; i++) {
    for(int j=0; j<nbRow; j++) {
      //threshold en luminance par carré du laucnhpad
      if(presenceSum[i][j] > presenceThresh) {
        fill(255, 255, 0, 25);
        //on envoi un message osc seulement si le statut du carré a changé 
        if(presenceSum[i][j] != lastPresenceSum[i][j]) {
          OscMessage myMessage = new OscMessage("/trig");
          myMessage.add(i); 
          myMessage.add(j); 
          myMessage.add(1); 
          oscP5.send(myMessage, myRemoteLocation); 
        }
        lastPresenceSum[i][j] = presenceSum[i][j];
      } else {
        noFill();
        //idem
       if(presenceSum[i][j] != lastPresenceSum[i][j]) {
          OscMessage myMessage = new OscMessage("/trig");
          myMessage.add(i); 
          myMessage.add(j); 
          myMessage.add(0); 
          oscP5.send(myMessage, myRemoteLocation); 
        }
        lastPresenceSum[i][j] = presenceSum[i][j];
      }
      rect(i*(width/nbCol), j*(height/nbRow), width/nbCol, height/nbRow);
    }
  }
}

// When a key is pressed, capture the background image into the backgroundPixels
// buffer, by copying each of the current frame's pixels into it.
void keyPressed() {
  video.loadPixels();
  arraycopy(video.pixels, backgroundPixels);
}
