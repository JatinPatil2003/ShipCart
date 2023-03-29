
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <string.h>
ESP8266WebServer server(80);
//String names;
//String price;
int quantity=0;
int Total_Cost = 0;
int command=0;

#define FIREBASE_HOST "makethon-b15dd-default-rtdb.firebaseio.com" //--> URL address of your Firebase Realtime Database.
#define FIREBASE_AUTH "rFPu6p3cDDku6pC8XDRuGh8LPNBsnUxiYbUVe6wP" //--> Your firebase database secret code.

#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router

//----------------------------------------SSID and Password of your WiFi router.
const char* ssid = "TUF_DASH_F15"; //--> Your wifi name or SSID.
const char* password = "Jatin$321"; //--> Your wifi password.
//----------------------------------------

LiquidCrystal_I2C lcd(0x27,16,2);
long randNumber;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.println("SMART CART      ");
  delay(500);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board

  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor.
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  server.on("/",HTTP_GET,sendStatus);
  server.on("/barcode",HTTP_POST,setbarcode);
  server.on("/nameprice",HTTP_POST,setnameprice);
  server.on("/namepriceqty",HTTP_POST,setnamepriceqty);
  server.onNotFound(handle_notFound);
  server.begin();
  //----------------------------------------

  //----------------------------------------Firebase Realtime Database Configuration.
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //----------------------------------------
  pinMode(A0,INPUT_PULLUP);
  
}

void sendStatus(){
  Serial.println("CONNECTION TRIGGERED !!!");
  server.send(200, "text/plain","I am Good");
}

void setbarcode(){
  if(!server.hasArg("code")){
    server.send(400);
  }
  else{
    String barcode = server.arg("code");
    server.send(200);
    command = analogRead(A0);
    if(command > 215 && command < 240){
      lcd.clear();
      lcd.print("Total Cost :");
      lcd.setCursor(0,1); 
      lcd.print("Rs ");
      lcd.setCursor(3,1);
      delay(1000);
      lcd.clear();
      lcd.print("   SMART CART   ");
    }
  }
}
void setnameprice(){
  if(!server.hasArg("name") && !server.hasArg("price")){
    server.send(400);
  }
  else{
    String names = server.arg("name");
    String price = server.arg("price");
    server.send(200);
    Serial.println("Item recived: " + names + " - " + price);
//    lcd.setCursor(0,1);
//    lcd.print("Item recived: " + names + " - " + price);
//    Serial.print
//    String path = "/"+names;
  Serial.println("add or remove:");
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("Add/Remove");//add remove display
  command = analogRead(A0);
  while(command < 100 || command >165){
    command = analogRead(A0);
    yield();
//    Serial.println(command);//223 316 495 1024
    if(command>100 && command < 105){
      Serial.println("Enter Quantity:");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter Quantity:");
      command = analogRead(A0);
      while(command != 1024){
        command = analogRead(A0);
        yield();
        if(command > 300 && command < 330){
          quantity += 1;
          delay(500);
          Serial.print(quantity);
          lcd.setCursor(1,1);
          lcd.print(quantity);
          lcd.print("   ");
        }
        else if(command > 480 && command < 510){
          quantity -= 1;
          delay(500);
          Serial.println(quantity);
          lcd.setCursor(1,1);
          lcd.print(quantity);
          lcd.print("   ");
        }
        
      }
      Serial.print("Final Quantity: ");
      Serial.println(quantity);
      String path = "A0BEF128/"+names;
      String prices = String(price.toInt() * quantity) + " " + quantity; 
      Total_Cost += prices.toInt();
      Firebase.setString(path,prices);
      Serial.println("updated value" + path + " " + prices);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Updated Quantity");
      quantity = 0;
      break;
    }
    else if(command>150 && command < 175){
      String path = "A0BEF128/" + names; 
      String curqu = Firebase.getString(path);
//      Serial.println(curqu.substring());
//      Serial.println(curqu);
//      Serial.println(curqu.length());
      String qua = curqu.substring(curqu.length()-2, curqu.length());
//      Serial.println(qua.toInt());
      int initialq = qua.toInt();
      Total_Cost -= initialq * price.toInt();
      Serial.println("Enter Quantity:");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter Quantity:");
      command = analogRead(A0);
      while(command != 1024){
        command = analogRead(A0);
        yield();
        if(command > 300 && command < 330){
          quantity += 1;
          delay(500);
          Serial.println(quantity);
          lcd.setCursor(1,1);
          lcd.print(quantity);
          lcd.print("   ");
        }
        else if(command > 480 && command < 510){
          quantity -= 1;
          delay(500);
          Serial.println(quantity);
          lcd.setCursor(1,1);
          lcd.print(quantity);
          lcd.print("   ");
        }
        
      }
      Serial.print("Final Quantity: ");
      Serial.println(quantity);
      Total_Cost += quantity * price.toInt();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Final Quantity: ");
      lcd.setCursor(1,1);
      lcd.print(quantity);
      lcd.print("   ");
      
      int totalq = initialq - quantity;
//      Firebase.remove(path);
      String prices = String(price.toInt() * totalq) + " " + totalq; 
      Firebase.setString(path,prices);
      Serial.println("updated value" + path + " " + prices);
      Serial.println("Removed value" + path);
      quantity = 0;
      break;
    }
  }
  Serial.println("Operation done sucessfully!");
  lcd.clear();
  lcd.print(" Operation done ");
  lcd.setCursor(0,1);
  lcd.print("  Sucessfully!  ");
  delay(1000);
  lcd.clear();
  lcd.print("   SMART CART   ");
  
  
  
  }
}

void setnamepriceqty(){
  if(!server.hasArg("name") && !server.hasArg("price")&& !server.hasArg("qty")){
    server.send(400);
  }
  else{
    String names = server.arg("name");
    String price = server.arg("price");
    String qty = server.arg("qty");
//    Serial.println("Item recived: " + names + " - " + price);
//    Serial.print
    String path = "A0BEF128/"+names;
  Serial.println(path);
  String quant = price + " " + qty;
  Firebase.setString(path,quant);
  Serial.println("updated value" + path + " " + qty);
    server.send(200);
  }
}

void handle_notFound(){
  server.send(404);
}

void loop() {
  // put your main code here, to run repeatedly:

  //----------------------------------------Send data in the form of random value data to the Firebase Realtime Database.
//  randNumber = random(100, 1000);
//  String dataSend = String(randNumber); 
//  Serial.print("Random Number : ");
//  Serial.println(dataSend);
//
//  Firebase.setString("RandomVal/Value",dataSend); //--> Command or code to send data or update data (String data type) to the Firebase Realtime Database.
//
//  // Conditions for handling errors.
//  if (Firebase.failed()) { 
//      Serial.print("Setting /Value failed :");
//      Serial.println(Firebase.error());  
//      delay(500);
//      return;
//  }
//
//  Serial.println("Setting successful");
//  Serial.println();
  //----------------------------------------

  //----------------------------------------Command or code to get data from Firebase Realtime Database.
//  /*
//  Serial.print("Value : ");
//  Serial.println(Firebase.getString("RandomVal/Value")); //--> Command or code to get data (String data type) from Firebase Realtime Database.
//
//  // Conditions for handling errors.
//  if (Firebase.failed()) { 
//      Serial.print("Getting /Value failed :");
//      Serial.println(Firebase.error()); 
//      delay(500); 
//      return;
//  }
//  
//  //----------------------------------------
//  */
//  delay(5000);
  
  // For other examples of FirebaseArduino, look in File -> Examples -> FirebaseArduino.

  server.handleClient();
//  command = analogRead(A0);
//  if(command > 215 && command < 240){
//    lcd.clear();
//    lcd.print("Total Cost :");
//    lcd.setCursor(0,1);
//    lcd.print("Rs ");
//    lcd.setCursor(3,1);
//    lcd.print(Total_Cost);
//    delay(1000);
//    lcd.clear();
//    lcd.print("   SMART CART   ");
//  }
  
}
