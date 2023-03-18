//ganancias del modelo en tiempo continuo
float kp = 1.0;
float ki = 8.0;
float kd = 0.5;
//Periodo de muestreo
float T = 0.01;
//Ganancias del model discreto
float K1;
float K2;
float K3;

void setup() {
  Serial.begin(2000000);
  // put your setup code here, to run once:
  K1 = kp+ki*T+kd/T;
  K2 = -kp-2.0*kd/T;
  K3 = kd/T;
}

float y[2]={0.0,0.0}; //salida y[0] salida actual   y[1] salida anterior
float e[3]={0.0,0.0,0.0};
//e[0] error actual  e[1] error anterior    e[2] error dos veces anterior
//e[k]               e[k-1]                 e[k-2]    
float referencia;
float retro; //retroalimentación

void loop() {
  //obtener la referencia
  referencia = 100; // analogRead(0)     Serial.read();
  //obtener la retroalimentación
  retro = analogRead(1); // lectura de un sensor por i2c, por spi, serie
  //calcular el error
  e[0]=referencia - retro;
  //calcular la salida del PID
  y[0]=K1*e[0]+K2*e[1]+K3*e[2]+y[1];

  //límitar la salida del PID por cuestiones físicas
  if(y[0]>255) y[0]=255;
  if(y[0]<-255)   y[0]=-255;

  //sacar la señal de control
  if(y[0]>0) // significa que es positivo
  {
  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);  //PARA QUE GIRE EN UN SENTIDO
  analogWrite(9,(int)y[0]);
  }
  else
  {
  digitalWrite(4,LOW);  // IN 1
  digitalWrite(5,HIGH); // IN 0
  analogWrite(9,(int)(-y[0])); // ENABLE DEL PUENTE H   
   }
  e[2]=e[1];
  e[1]=e[0];
  y[1]=y[0]; 
  Serial.println(String(referencia)+ "  "+ String(retro)+ "  " +
                  String(y[0])+ "   " + String(e[0]));
  delay((int)1000*T); 
}
