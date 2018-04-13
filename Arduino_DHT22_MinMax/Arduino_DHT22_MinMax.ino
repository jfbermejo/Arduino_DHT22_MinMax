#include <DHT.h>

// Conexiones:
// Pin 1 del sensor (a la izquierda) a +5V de la placa
// Pin 2 del sensor a la entrada de datos digital que se quiera utilizar
// Pin 3 del sesnor, libre
// Pin 4 del sensor (a la derecha) a GND (tierra)
// Conectar una resistencia 10K desde pin datos a alimentación (vcc) del sensor (si no está incluida en encapsulado)

#define DHTPIN 2        // Pin donde conectamos la salida datos del sensor, pin 2 en este caso
#define DHTTYPE DHT22   // Tipo de sensor DHT22 (AM2303)

// Declaramos el sensor
DHT dht( DHTPIN , DHTTYPE );

// Declaramos las variables para almacenar los máximos y mínimos
volatile float hMax = -1.0;
volatile float hMin = 999.9;
volatile float tMax = -1.0;
volatile float tMin = 999.9;
volatile float hicMax = -1.0;
volatile float hicMin = 999.9;

void setup() {
  // Configuración inicial

  Serial.begin( 9600 );
  Serial.println( "Test de uso del sensor DHT22" );
  Serial.println( "Midiendo temperatura y humedad" );

  // Iniciamos el sensor
  dht.begin();
}

void loop() {

  // Obtenemos las medidas
  float h = dht.readHumidity();           // Lectura de humedad en porcentaje
  float t = dht.readTemperature();        // Lectura de temperatura en Celsius
  float f = dht.readTemperature( true );  // Lectura de temperatura en Fahrenheit

  // Comprobamos si falló alguna medición, en cuyo caso vuelve a intentarlo
  if( isnan(h) || isnan(t) || isnan(f) ){
    
    Serial.println( "Se produjo un fallo en la lectura del sensor DHT!" );
    
  } else {
    
    // Calculamos el índice de calor (se le envía la temperatura en Fahrenheit)
    float hif = dht.computeHeatIndex( f , h );
    float hic = (hif - 32) / 1.8;

    // Comprobamos los rangos de humedad
    if( h > hMax ){
      hMax = h;
    }
    
    if( h < hMin ){
      hMin = h;
    }

    // Comprobamos los rangos de temperatura en Celsius
    if( t > tMax ){
      tMax = t;
    }
    
    if( t < tMin ){
      tMin = t;
    }

    // Comprobamos los rangos de índice de calor
    if( hic > hicMax ){
      hicMax = hic;
    }
    
    if( hic < hicMin ){
      hicMin = hic;
    }

    // Mostramos por puerto serie la información
    Serial.println( "================================================================" );

    Serial.print( " T:    " );
    Serial.print( t );
    Serial.print( " ºC\t\t" );

    Serial.print( "RH:    " );
    Serial.print( h );
    Serial.print( " %\t\t" );
    
    Serial.print( "HI:    " );
    Serial.print( hic );
    Serial.print( " ºC\n" );

    Serial.println( "----------------------------------------------------------------" );

    Serial.print( " Tmin: " );
    Serial.print( tMin );
    Serial.print( " ºC\t\t" );

    Serial.print( "RHmin: " );
    Serial.print( hMin );
    Serial.print( " %\t\t" );
    
    Serial.print( "HImin: " );
    Serial.print( hicMin );
    Serial.print( " ºC\n" );

    Serial.print( " Tmax: " );
    Serial.print( tMax );
    Serial.print( " ºC\t\t" );

    Serial.print( "RHmax: " );
    Serial.print( hMax );
    Serial.print( " %\t\t" );
    
    Serial.print( "HImax: " );
    Serial.print( hicMax );
    Serial.print( " ºC\n" );

    Serial.println( "================================================================" );
    Serial.print( "\n\n" );

    // Esperamos 10 segundos hasta la siguiente medición
    // El sensor DHT22 puede realizar mediciones cada 2 segundos
    delay(10000);
    
  }

}
