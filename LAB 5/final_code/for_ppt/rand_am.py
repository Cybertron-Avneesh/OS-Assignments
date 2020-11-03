import time 
import random
import string
import pyautogui 
 
  
# makes program execution pause for 10 sec 
# listInputs = ["1", "y", "aaaa",     
#               "5", "n",
#               "17", "y", "bbbb",
#               "70", "n", 
#               "2", "y", "cccc",
#               "2", "n", 
#               "19", "n",
#               "22", "n", 
#               "35", "y", "dddd",
#               "32", "y", "eeee",
#               "46", "n",
#               "67", "y", "ffff",
#               "70", "y", "gggg",
#               "3", "n",
#               "4", "y", "hhhh"]
            
N = 20 # number of elements in ref string
all_rec = []
rec_no_list = random.sample(range(1,94), N)

for rec in rec_no_list:
    all_rec.append(str(rec))
    to_be_changed = random.choice(['y','n','y', 'n', 'y', 'n', 'y', 'n', 'y', 'n', 'y', 'n'])
    if(to_be_changed == 'y'):
        all_rec.append(to_be_changed)
        change_Str = ''.join(random.choice(string.ascii_lowercase) for i in range(4))
        all_rec.append(change_Str)
    else:
        all_rec.append(to_be_changed)

all_rec.append(str(-1))

print(all_rec)

time.sleep(4)

pyautogui.click()
for item in all_rec:
    pyautogui.typewrite(item)
    pyautogui.press('enter')
    time.sleep(0.5)