import firebase_admin
import socket
import serial
import threading
import json
from firebase_admin import credentials, firestore, storage
import paho.mqtt.client as mqtt
from datetime import datetime
from picamera import PiCamera
import time
from io import BytesIO
'''
Falta obtener el UID por MQTT desde el movil, asi publicamos nights data en usuario personalizado
Falta que no sea un update sino que cree un nuevo night
Falta una funcion para enviar por MQTT al movil, que ya acabo la noche
'''

cred = credentials.Certificate('firebase-adminsdk.json')
firebase_admin.initialize_app(cred, {
    'storageBucket': 'gs://hypnos-gti.appspot.com'
})

storage_bucket = storage.bucket()

# Referencia a la colección en la que deseas agregar datos
db = firestore.client()
documento_referencia = db.collection('users').document('R279SubMuPfIJf608GXGWbFOoTC2').collection('nightsData').document('y29w5QHPKWj4tYD7YHQG')
uid_usuario = "lr3SPEtJqt493dpfWoDd"

# Configura el socket UDP
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp_host = '0.0.0.0'  # Escucha en todas las interfaces de red
udp_port = 6230       # Elige un número de puerto disponible
udp_socket.bind((udp_host, udp_port))

# Configura el objeto serial
ser = serial.Serial(
    port='/dev/ttyUSB0',  # Puerto serial en Raspberry Pi (puede variar)
    baudrate=115200,       # Velocidad de baudios, asegúrate de que coincida con la configuración del ESP32
    timeout=1              # Tiempo de espera para la lectura serial
)

# Configuración de MQTT
mqtt_broker_address = "test.mosquitto.org"  # Cambia esto con la IP de tu Raspberry Pi
mqtt_topic = "hypnos_m5stack_topic"
mqtt_client = mqtt.Client()
uid_usuario = None

# Configuración de la cámara
camera = PiCamera()

# Variables para almacenar los datos de UDP y UART
datos_udp = None
datos_uart = None
datos_mqtt = None

# Array para almacenar los archivos de imagen
imagenes = []

def tomar_fotos():
    for _ in range(3):  # Tomar 3 fotos
        stream = BytesIO()
        camera.capture(stream, 'jpeg')
        imagenes.append(stream.getvalue())
        time.sleep(4)
        
    #Ver si esto afecta en algo malo al codigo, si debemos cerrarla despues de llamar a la funcion
    camera.close()
    
def subir_imagenes_storage():
    try:
        for i, imagen in enumerate(imagenes):
            nombre_imagen = f'imagen_{i}_{int(time.time())}.jpg'

            # Construir la ruta completa en el bucket de almacenamiento
            ruta_storage = f'users/{uid_usuario}/{nombre_imagen}'

            # Obtener una referencia al objeto de almacenamiento
            storage_ref = storage_bucket.blob(ruta_storage)

            # Subir la imagen al almacenamiento
            storage_ref.upload_from_string(imagen, content_type='image/jpeg')

            print(f'Imagen {i} subida correctamente a Firestore con la ruta: {ruta_storage}')

    except Exception as e:
        print(f"Error al subir imágenes a Firestore: {e}")

def recibir_datos_udp():
    global datos_udp
    try:
        while True:
            data, addr = udp_socket.recvfrom(6230)
            data = data.decode('utf-8').strip()
            snoreScore = ""
            if data:
                try:
                    mensaje_json = json.loads(data)
                    if "snoreAmount" in mensaje_json:
                        if mensaje_json["snoreAmount"] <= 1:
                           snoreScore = "Buena"
                        elif mensaje_json["snoreAmount"] >=4:
                           snoreScore = "Mala"
                        else:
                           snoreScore = "Media"
                        datos_udp = {
                            'temperature': mensaje_json["averageTemperature"],
                            'snore': snoreScore
                        }
                        print('Datos UDP recibidos correctamente:', datos_udp)
                        break
                except json.JSONDecodeError:
                    print('Mensaje UDP no es un JSON válido:', data)

    except KeyboardInterrupt:
        udp_socket.close()
        print("\nSocket UDP cerrado.")

def recibir_datos_uart():
    global datos_uart
    try:
        while True:
            data = ser.readline().decode('utf-8').strip()

            # Verificar si se recibió algún dato antes de agregarlo a la colección
            if data.isdigit():
                print(data)
                datos_uart = int(data)//1000
                print('Datos UART recibidos correctamente:', datos_uart)
                break

    except KeyboardInterrupt:
        ser.close()
        print("\nComunicación serial cerrada.")

mqtt_loop_activo = True


#AQUI HAY QUE TENER DOS OPCIONES UNA PARA RECIBIR EL UID Y OTRA PARA RECIBIR LOS DATOS DEL M5
def on_message(client, userdata, message):
    global datos_mqtt
    payload = message.payload.decode('utf-8')
    datos_mqtt = payload#json.loads(payload)
    print('Datos MQTT recibidos correctamente:', datos_mqtt)
    mqtt_client.disconnect()

    # Desactivar el bucle loop_forever() al recibir un mensaje
    #mqtt_loop_activo = False

def iniciar_hilo_mqtt():
    mqtt_client.connect(mqtt_broker_address)
    mqtt_client.subscribe(mqtt_topic)
    mqtt_client.on_message = on_message
    mqtt_client.loop_forever()

    # Bucle loop_forever() con la condición de la variable de control
    '''
    while mqtt_loop_activo:
        mqtt_client.loop(timeout=1.0)

    '''
def escribir_en_firestore():
    global datos_udp, datos_uart, datos_mqtt
    if datos_udp is not None and datos_uart is not None and datos_mqtt is not None:
        # Combinar datos de UDP, UART y MQTT
        datos_combinados = {
            'breathing': datos_udp["snore"],
            'date':datetime.now(),
            'temperature':datos_udp["temperature"],
            'time': datos_uart,
            'score': int(datos_mqtt)
        }

        documento_referencia.update(datos_combinados)
        print('Datos combinados agregados correctamente a Firestore en Firebase.')
        
        subir_imagenes_storage()


thread_udp = threading.Thread(target=recibir_datos_udp)
thread_uart = threading.Thread(target=recibir_datos_uart)
thread_mqtt = threading.Thread(target=iniciar_hilo_mqtt)
thread_foto = threading.Thread(target=tomar_fotos)

thread_udp.start()
thread_uart.start()
thread_mqtt.start()
thread_foto.start()

thread_udp.join()
thread_uart.join()
thread_mqtt.join()
thread_foto.join()

escribir_en_firestore()
