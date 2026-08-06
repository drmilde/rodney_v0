import cv2

def main():
    # 0 is usually the default built-in webcam or the first connected USB camera.
    # If you have multiple cameras, try changing this to 1, 2, etc.
    camera_index = 0
    
    # Initialize the video capture object
    cap = cv2.VideoCapture(camera_index)
    
    # Check if the webcam was opened successfully
    if not cap.isOpened():
        print(f"Error: Could not open video device at index {camera_index}")
        return

    print("Camera feed started. Press 'q' on your keyboard to quit.")

    while True:
        # Capture frame-by-frame
        # 'ret' is a boolean (True if frame reading was successful)
        # 'frame' is the actual image array (NumPy array)
        ret, frame = cap.read()
        
        if not ret:
            print("Error: Failed to grab frame.")
            break
            
        # Display the resulting frame in a window named 'Live USB Camera Feed'
        cv2.imshow('Live USB Camera Feed', frame)
        
        # Wait for 1 millisecond and check if the user pressed the 'q' key
        # 0xFF masks the integer to get the actual ASCII value
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # When everything is done, release the capture hardware and destroy all windows
    cap.release()
    cv2.destroyAllWindows()
    print("Camera feed closed successfully.")

if __name__ == "__main__":
    main()