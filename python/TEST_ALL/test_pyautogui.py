import pyautogui

cur_pos = pyautogui.position()

goto_pos=pyautogui.locateOnScreen("/Users/miaozw/Documents/00001.png")

# pyautogui.moveTo(pyautogui.center(goto_pos),duration=2)
print(goto_pos.top)
center_pos = pyautogui.center(goto_pos)
# print(pyautogui.size())

pyautogui.moveTo(x=center_pos.x/2,y=center_pos.y/2,duration=1)