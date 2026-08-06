import time
import serial


def serial_communication_example():
    # --- Configuration ---
    # Change 'COM3' to whichever port your device is connected to (e.g., 'COM1', 'COM4')
    port_name = "COM5"
    baud_rate = 9600  # Common baud rates: 9600, 115200

    try:
        # --- Open the Serial Port ---
        # The timeout parameter prevents the script from freezing forever if no data is received
        print(f"Opening {port_name} at {baud_rate} baud...")
        ser = serial.Serial(port_name, baudrate=baud_rate, timeout=1)

        # Give Windows a moment to stabilize the connection
        time.sleep(2)

        if ser.is_open:
            print(f"Connected successfully to {port_name}!")

            # --- 1. Writing Data (Sending a String) ---
            message_to_send = "Hello Device\n"

            # Serial ports transmit bytes, not raw Python strings.
            # We must encode the string into bytes (usually 'utf-8' or 'ascii').
            ser.write(message_to_send.encode("utf-8"))
            print(f"Sent: {message_to_send.strip()}")

            # Short pause to allow the connected device to receive and process the data
            time.sleep(0.5)

            # --- 2. Reading Data (Receiving a String) ---
            # Check if there is data waiting in the hardware buffer
            if ser.in_waiting > 0:
                # Read a line of data (stops at a newline character '\n')
                raw_data = ser.readline()

                # Decode the bytes back into a human-readable Python string
                decoded_message = raw_data.decode("utf-8").strip()
                print(f"Received: {decoded_message}")
            else:
                print("No response received from the device (Timeout).")

            # --- 3. Close the Port ---
            ser.close()
            print(f"Port {port_name} closed safely.")

    except serial.SerialException as e:
        print(f"Serial Error: Could not open or communicate with {port_name}.")
        print(f"Details: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")



def read_loop():
    # --- Configuration ---
    # Change 'COM3' to whichever port your device is connected to (e.g., 'COM1', 'COM4')
    port_name = "COM5"
    baud_rate = 9600  # Common baud rates: 9600, 115200

    try:
        # --- Open the Serial Port ---
        # The timeout parameter prevents the script from freezing forever if no data is received
        print(f"Opening {port_name} at {baud_rate} baud...")
        ser = serial.Serial(port_name, baudrate=baud_rate, timeout=1)

        # Give Windows a moment to stabilize the connection
        time.sleep(2)

        if ser.is_open:
            print(f"Connected successfully to {port_name}!")

        while True:
                # --- 2. Reading Data (Receiving a String) ---
                # Check if there is data waiting in the hardware buffer
                if ser.in_waiting > 0:
                    # Read a line of data (stops at a newline character '\n')
                    raw_data = ser.readline()

                    # Decode the bytes back into a human-readable Python string
                    decoded_message = raw_data.decode("utf-8").strip()
                    print(f"Received from ESP32: {decoded_message}")
                # else:
                #    print("No response received from the device (Timeout).")

                # Short pause to allow the connected device to receive and process the data
                time.sleep(0.01)

                # --- 1. Writing Data (Sending a String) ---
                message_to_send = "Hello Device\n"

                # Serial ports transmit bytes, not raw Python strings.
                # We must encode the string into bytes (usually 'utf-8' or 'ascii').
                ser.write(message_to_send.encode("utf-8"))
                print(f"Sent: {message_to_send.strip()}")

                time.sleep(0.01)

    except serial.SerialException as e:
        print(f"Serial Error: Could not open or communicate with {port_name}.")
        print(f"Details: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")




if __name__ == "__main__":
    #serial_communication_example()
    read_loop()