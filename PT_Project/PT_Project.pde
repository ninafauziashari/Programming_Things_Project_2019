import g4p_controls.*;
import processing.serial.*;

Serial port; //Create object from Serial class
String val; //Data received from serial port   

public void setup(){
  createGUI();
  size(700, 500, JAVA2D);
  String portName = "COM3";
  port = new Serial(this, portName, 9600); 
}

public void draw(){
  //background(230, );
  if (port.available() >0){
    val = port.readStringUntil('\n');
    if (val != null){
      consoleTextArea.appendText(val);
    }
  }
}
