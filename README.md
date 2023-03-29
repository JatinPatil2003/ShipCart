# SmartCart
Project made in makethon to solve Real World Problem of reducing time in Billing procedure


## Solution
Developed a product which works on Firebase RealTime Database to keep track of object in track of items in cart.
Customer will just scan the barcode of the image from esp32cam and then that product quantity will be asked by the system
and finally it will add in our realtime data-base.
We will have added the feature of removing an item so if the user wants to remove excess items from the cart he can press the remove button on the device.
Once the shopping is done, the customer can take the cart to the billing counter. The RFID present at the counter scans the RFID card present on the device and accesses the cart of the user from the real time database.
Once scanned the counter generates a receipt of the bill and the shopping is completed.
To make the cart available for the next user the cart also check-out the user by the existing cart.
Thus, using this system we can make the billing system more time-efficient.
