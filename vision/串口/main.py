import cv2 as cv
import numpy as np
import uart

yolo_tiny_model = "yolov4-tiny.weights";
yolo_tiny_cfg = "yolov4-tiny.cfg";
capture = cv.VideoCapture(0)

# Load names of classes
classes = None
with open("coco.names", 'rt') as f:
    classes = f.read().rstrip('\n').split('\n')

# load tensorflow model
net = cv.dnn.readNetFromDarknet(yolo_tiny_cfg, yolo_tiny_model)
# set back-end
net.setPreferableBackend(cv.dnn.DNN_BACKEND_OPENCV)
net.setPreferableTarget(cv.dnn.DNN_TARGET_CPU)

#image = cv.imread('dog.jpg')
while (True) :
    ref, image = capture.read()
    image = cv.flip(image, 1)
    h, w = image.shape[:2]

    blobImage = cv.dnn.blobFromImage(image, 1.0 / 255.0, (416, 416), None, True, False);
    outNames = net.getUnconnectedOutLayersNames()
    net.setInput(blobImage)
    outs = net.forward(outNames)

    t, _ = net.getPerfProfile()
    fps = 1000 / (t * 1000.0 / cv.getTickFrequency())
    label = 'FPS: %.2f' % fps
    cv.putText(image, label, (0, 15), cv.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0))

    # 绘制检测矩形
    classIds = []
    confidences = []
    boxes = []
    for out in outs:
        for detection in out:
            scores = detection[5:]
            classId = np.argmax(scores)
            confidence = scores[classId]
            # numbers are [center_x, center_y, width, height]
            if confidence > 0.05:
                center_x = int(detection[0] * w)
                center_y = int(detection[1] * h)
                width = int(detection[2] * w)
                height = int(detection[3] * h)
                left = int(center_x - width / 2)
                top = int(center_y - height / 2)
                classIds.append(classId)
                confidences.append(float(confidence))
                boxes.append([left, top, width, height])

    # 使用非最大抑制
    indices = cv.dnn.NMSBoxes(boxes, confidences, 0.5, 0.4)
    #print(indices)
    for i in indices:
        box = boxes[i]
        left = box[0]
        top = box[1]
        width = box[2]
        height = box[3]
        cv.rectangle(image, (left, top), (left + width, top + height), (0, 0, 255), 2, 8, 0)
        cv.putText(image, classes[classIds[i]], (left, top),cv.FONT_HERSHEY_SIMPLEX, 1.0, (255, 255, 0), 2)
        if classes[classIds[i]] == "person":
            uart.usart_out(6, 0, 0)
        else:
            uart.usart_out(0, 0, 0)


    cv.namedWindow("YOLOv4-tiny-Detection-Demo", cv.WINDOW_NORMAL)
    cv.imshow('YOLOv4-tiny-Detection-Demo', image)
    c = cv.waitKey(30) & 0xff
    if c == 27:
        capture.release()
        break
cv.destroyAllWindows()