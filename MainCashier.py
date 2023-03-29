import serial.tools.list_ports
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db
from prettytable import PrettyTable

ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()

# portsList = []

# for onePort in ports:
#     portsList.append(str(onePort))
#     print(str(onePort))

# val = input("Select Port: COM")

# for x in range(0,len(portsList)):
#     if portsList[x].startswith("COM" + str(val)):
#         portVar = "COM" + str(val)
#         print(portVar)

serialInst.baudrate = 9600
serialInst.port = "COM7"
serialInst.open()


while True:
    while True:
        if serialInst.in_waiting:
            packet = serialInst.readline()
            cartuid = packet.decode('utf').rstrip('\n')
            break

    # print(cartuid[:-1])
    cartuid = cartuid[:-1].split(".")
    cartstr =""
    for i in cartuid:
        cartstr += i
    print(cartstr)
    cartstr = "/" + cartstr




    #Connecting the real time database with Python code to access the final shopping(Cart)

    cred = credentials.Certificate('C:/Users/abhin/OneDrive/Desktop/hack/biller/makethonsecret.json')
    firebase_admin.initialize_app(cred,{
        'databaseURL':"https://makethon-b15dd-default-rtdb.firebaseio.com/"
    })

    Ref = db.reference(cartstr)
    arr = Ref.get()
    print(arr)

    k = list(arr.keys())
    v = list(arr.values())

    q = [] #quantity
    p = [] #price per item
    t = [] #total price
    sum1 = 0 #counter for total bill
    sum2 = 0 #counter for total quantity
    for i in range(len(v)):
        s = v[i].split()
        t.append(int(s[0]))
        q.append(int(s[1]))
        p.append(int(s[0])/int(s[1]))
        sum1+=int(s[0])
        sum2+=int(s[1])

    #Printing the final bill

    T = PrettyTable(['Type','Price','Quantity','Total'])
    for i in range(len(v)):
        T.add_row([k[i],p[i],q[i],t[i]])
    T.add_row(["-"*9,"-"*7,"-"*8,"-"*7])
    T.add_row(['Total:',' ',sum2,sum1])
    print(T)
    print("Your Total Bill is: ",sum1)
    print("Happy Shopping!")

    #Once the shopping is done clearing the cart and making it available for the next customer
    ref = db.reference(cartstr)
    for key,value in arr.items():
        ref.child(key).set({})