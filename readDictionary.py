# import libraries
import sys
import string
from tkinter import *

# list to add all words from dictionary
words = []

# create Tk window
root = Tk()
root.title("Dictionary Search Engine")
root.configure(bg="black")

# search box for user input to search a word in dictionary
searchEntry = Entry(root, width=40)
searchEntry.grid(row=0,column=0)

# Text widget to display results from user query
console = Text()

# search button function call 
def searchButtonExec():	
	# erase console content
	console.delete(1.0,END)
	
	# text variable store result as string from user input	
	text = searchEntry.get()
	
	# process_text function will search dictionary with user input
	process_text(text)

# Button widget for search  	
searchButton = Button(root, text="search", command=searchButtonExec)
searchButton.grid(row=0,column=1)


console.grid(row=1)

# add words from dictionary into list "words"
def readDictionary():
	global words
	file = open('dictionary.txt','r')	
	
	for line in file:
		line = line.strip('\n')
		words.append(line)
	file.close()	

# process_text finds words in dictionary that contain string in argument input_text 	
def process_text(input_text):		
		#wordmatchcnt keeps track of word displayed on as single word in line on the text widget
		wordmatchcnt = 1
		# iterate over the list of words
		for word in words:
			# if word contains sub-string from user query 
			if word.find(input_text) >= 0:
				# x is the starting index of substring in word
				x = word.find(input_text)
				
				# add word to end of text widget and append newline character
				console.insert(END,str(word))
				console.insert(END,'\n')
				
				# y is end index of substring in word
				y = x + len(input_text)
				
				# startcolorindex, endcolorindex is used for highlighting substring in word
				startcolorindex = str(wordmatchcnt) + "." + str(x)
				endcolorindex = str(wordmatchcnt) + "." + str(y)
				
				# highlight text in word
				console.tag_add("start", startcolorindex, endcolorindex)
				console.tag_config("start", background="black", foreground="yellow")
				
				wordmatchcnt = wordmatchcnt + 1																
readDictionary()	
# size of window
root.geometry("800x600")
root.mainloop()
