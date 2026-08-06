import cv2
from ultralytics import YOLO

def main():
    # 1. Load the lightweight YOLO11 Nano model
    # It will automatically download the 'yolo11n.pt' file on its first run
    model = YOLO("yolo11n.pt")
    
    # 2. Initialize the USB Camera (0 is usually the default camera)
    cap = cv2.VideoCapture(0)
    
    if not cap.isOpened():
        print("Error: Could not open the USB camera.")
        return

    print("YOLO11 Live Person Detection Started. Press 'q' to exit.")

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Error: Failed to grab frame.")
            break
            
        # 3. Run inference on the current frame
        # verbose=False keeps the terminal clean from frame-by-frame outputs
        results = model(frame, verbose=False)
        
        # 4. Extract boxes from the first result
        result = results[0]
        boxes = result.boxes
        
        for box in boxes:
            # Class ID (e.g., 0 for person, 1 for bicycle, etc.)
            cls_id = int(box.cls[0].item())
            
            # Filter specifically for 'person' (Class 0 in COCO dataset)
            if cls_id == 0:
                # Extract coordinates: x1, y1 (top-left) and x2, y2 (bottom-right)
                x1, y1, x2, y2 = map(int, box.xyxy[0].tolist())
                
                # Get the confidence score
                confidence = box.conf[0].item()
                
                # Draw the bounding box (BGR color format: Green is (0, 255, 0))
                # Thickness is set to 2 pixels
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
                
                # Add text label showing 'Person' and confidence percentage
                label = f"Person: {confidence:.2%}"
                cv2.putText(
                    frame, 
                    label, 
                    (x1, max(y1 - 10, 10)), # Position text slightly above the box
                    cv2.FONT_HERSHEY_SIMPLEX, 
                    0.5, 
                    (0, 255, 0), # Green text
                    2
                )
                
        # 5. Display the live feed window
        cv2.imshow("YOLO11 Live Person Detection", frame)
        
        # Break out of the loop if 'q' key is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the resources
    cap.release()
    cv2.destroyAllWindows()
    print("Application closed.")

if __name__ == "__main__":
    main()