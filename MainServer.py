import cv2
from pyzbar.pyzbar import decode
import requests
import time

camera_id = 'http://192.168.137.173:81/stream'
# camera_id = 0
delay = 1
window_name = 'OpenCV pyzbar'
nowtime=0
prevtime =0

Inventory = {
  "7622201150419": ["5_STAR_CHOCOLATE", "20"],
  "8902433003165": ["ORBIT_KACCHA_MANGO_CHEWINGHUM","10"],
  "8902442228597": ["YOUVA_NOTEBOOK", "60"],
  "8906073782480": ["DOMS_BLUE_NOTEBOOK", "160"]
}

Mycart = {
	"5_STAR_CHOCOLATE": ["20", "0"],
	"ORBIT_KACCHA_MANGO_CHEWINGHUM": ["10", "0"],
	"YOUVA_NOTEBOOK":["60", "0"],
	"DOMS_BLUE_NOTEBOOK":["160","0"]
}

cap = cv2.VideoCapture(camera_id)

while True:
    ret, frame = cap.read()
    image = frame

    if ret:
        for d in decode(frame):
            s = d.data.decode()
            print(s)
            frame = cv2.rectangle(frame, (d.rect.left, d.rect.top),
                                  (d.rect.left + d.rect.width, d.rect.top + d.rect.height), (0, 255, 0), 3)
            frame = cv2.putText(frame, s, (d.rect.left, d.rect.top + d.rect.height),
                                cv2.FONT_HERSHEY_SIMPLEX, 2, (0, 0, 255), 2, cv2.LINE_AA)
            if(s in Inventory.keys()):
                [item_name, item_price] = Inventory[s]
                print(Inventory[s])
            print(item_name)
            print(item_price)
            
            # nowtime = time.time()                                               
            # if(nowtime - prevtime > 10):
            #     Mycart[item_name][1] = str(int(Mycart[item_name][1]) + 1)
            #     Mycart[item_name] = [str(int(item_price) * int(Mycart[item_name][1])), Mycart[item_name][1]]
            #     url2 = "http://192.168.137.239/nameprice?name=" + item_name + "&price=" + item_price
            #     url3 = "http://192.168.137.239/namepriceqty?name=" + item_name + "&price=" + Mycart[item_name][0] + "&qty=" + Mycart[item_name][1]
            #     requests.post(url2)
            #     prevtime = nowtime
            Mycart[item_name][1] = str(int(Mycart[item_name][1]) + 1)
            Mycart[item_name] = [str(int(item_price) * int(Mycart[item_name][1])), Mycart[item_name][1]]
            url2 = "http://192.168.137.239/nameprice?name=" + item_name + "&price=" + item_price
            url3 = "http://192.168.137.170/namepriceqty?name=" + item_name + "&price=" + Mycart[item_name][0] + "&qty=" + Mycart[item_name][1]
            requests.post(url2)
            count =0
            while(count<20):
                ret,image = cap.read()
                count +=1
                print(count)
                frame = image
                cv2.imshow(window_name, frame)
        cv2.imshow(window_name, frame)
        
        # time.sleep(3)

    if cv2.waitKey(delay) & 0xFF == ord('q'):
        break

cv2.destroyWindow(window_name)