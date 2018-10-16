# STAT/CS 287
# HW 01
#
# Name: Chia-Chun Chao
# Date: Tuesday, September 12, 2017


print("******************** HW01: Problem 1/3 ********************")
"""
Python's set data structure supports mathematical set operations (union, intersection, difference, etc.)
1. Using online resources, IPython exploration, and other means, learn how to use Python sets. How do sets differ from dictionaries? 
2. Write a function similarity(A,B) that computes a similarity between two sets A and B , defined as:
    similarity(A,B)=|A∩B|/|A∪B|
    where |S| is the cardinality of set S, A∩B is the intersection of sets A and B, and A∪B is the union of sets A and B.
Call your similarity function on two or three small example sets that you have added to your .py file.
"""

def similarity(A,B):
    """
    Write a function similarity(A,B) that computes a similarity between two sets A and B , defined as:
        similarity(A,B)=|A∩B|/|A∪B|
        where |S| is the cardinality of set S, A∩B is the intersection of sets A and B, and A∪B is the union of sets A and B.
    """

    intersection = len(A.intersection(B))
    union = len(A.union(B))
    similarity = intersection/union #similarity(A,B)=|A∩B|/|A∪B|
    print("A =", A, "\nB =", B, "\nSimilarity =", intersection, "/", union, "=", similarity, "\n")
    
    pass


#Call your similarity function on two or three small example sets
A = set([1,2,3,4,5,6,7,8,9,0])
B = set([2,4,6,8,0])
C = set([1,5,8,11,20,35,7,2,9,17])
D = set(['a','b','c','d','e','f','g','h','i','j','k'])
E = set(['a','e','i','o','u'])

similarity(A, B)
similarity(A, C)
similarity(D, E)



print("******************** HW01: Problem 2/3 ********************")
"""
Flipping a biased random coin.
Using the python random module, write function coin_flip(p) that takes a number  0<p<10<p<1  and returns either heads or tails. 
If p=1/2 then both heads and tails are equally likely and the coin is fair. If p=0.9 then there is a 90% chance of a head and only a 10% chance of a tail.
You can use 'H' and 'T' to represent heads and tails, respectively.
Now, for p in [0.2,0.4,0.6,0.8]:
1. Flip this coin 1000 times, storing each heads or tails result to a list.
2. Compute the run statistics for heads for this coin flip history. A run of heads is a contiguous sequence of all heads. 
    For example, ['H','H','H','T','H','T','H','H'], has a run of 3, then a run of 1, then a run of 2. The run statistics count how many runs there are of length 1,2,3,4,5, etc.
To be clear: do the above two steps once for each value of p. Meaning, you set p = 0.2, flip 1000 coins, etc., then set p = 0.4, flip 1000 coins, etc.
"""
   
import random

def coin_flip(p):
    """
    Using the python random module, write function coin_flip(p) that takes a number  0<p<10<p<1  and returns either heads or tails. 
    If p=1/2 then both heads and tails are equally likely and the coin is fair. If p=0.9 then there is a 90% chance of a head and only a 10% chance of a tail.
    You can use 'H' and 'T' to represent heads and tails, respectively.
    """
   
    if random.random() < p: #random.random() returns the next random floating point number in the range [0.0, 1.0)
        return('H')
    else:
        return('T')
            
  
"""    
def calculate_length_list(flipList):
    #Calculate the length and store the result in a list
    length = 0
    lengthList = []     
    for flip in flipList:
        if flip == 'H': #If it is a 'H', increment the length.
            length += 1
        elif length != 0: #If it is a 'T' and the previous flip is 'H', start a new count.
            lengthList.append(length)
            length = 0
    if length != 0: #If the last flip is 'H', also append the length to the list.
        lengthList.append(length)

    lengthList.sort()   
    print (lengthList)
"""    
 
def calculate_length_dict(flipList):   
    """
    Calculate the length and store the result in a dictionary
    """
    lengthDict = {} 
    length = 0
    for flip in flipList:
        if flip == 'H': #If it is a 'H', increment the length.
            length += 1
        elif length != 0: #If it is a 'T' and the previous flip is 'H', start a new count and store the length.
            if length in lengthDict: #If the length has appeared, increment the count.
                lengthDict[length] += 1
            else: #This is the first time that this length appears.
                lengthDict[length] = 1
            length = 0
    if length != 0: #If the last flip is 'H', also add the length to the dictionary.
        if length in lengthDict: #If the length has appeared, increment the count.
            lengthDict[length] += 1
        else: #This is the first time that this length appears.
            lengthDict[length] = 1
    
    for key in sorted(lengthDict.keys()):
        print("length", key, ":", lengthDict[key], "times")
    print("All the others are 0.\n")
    
    pass


#for p in [0.2,0.4,0.6,0.8]
P = [0.2,0.4,0.6,0.8]
for p in P:
    print("p =", p)
    #Run 1000 times and store each result to a list
    flipList = []
    for i in range(1000): 
        flipList.append(coin_flip(p))
    calculate_length_dict(flipList)
    


print("******************** HW01: Problem 3/3 ********************")
"""
In the template there is a function that downloads and parses A Tale of Two Cities from Project Gutenberg. 
This function returns a list of all words in the book. Study how this function works.
1. Write code that takes the word list and counts the number of times each word occurs.
2. Print the 100 most common words and the number of times each occur, in descending order.
For step 1, you must use and document an appropriate and efficient data structure. 
    The final code should require only a few seconds to complete. 
    Please use .sort() or sorted() in your solution.
Bonus: Try to find a special python library or package to do this task with quickly. 
    Please use this library only after you have implemented your own "homegrown" solution.
"""
import urllib.request
from string import punctuation

def words_of_book():

       
    """Download `A tale of two cities` from Project Gutenberg. Return a list of
    words. Punctuation has been removed and upper-case letters have been
    replaced with lower-case.
    """

    url = "http://www.gutenberg.org/files/98/98.txt"
    req = urllib.request.urlopen(url)
    charset = req.headers.get_content_charset()
    raw = req.read().decode(charset)

    raw = raw[750:] # The first 750 or so characters are not part of the book.

    # Loop over every character in the string, keep it only if it is NOT
    # punctuation:
    exclude = set(punctuation) # Keep a set of "bad" characters.
    list_letters_noPunct = [ char for char in raw if char not in exclude ]

    # Now we have a list of LETTERS, *join* them back together to get words:
    text_noPunct = "".join(list_letters_noPunct)
    # (http://docs.python.org/2/library/stdtypes.html#str.join)

    # Split this big string into a list of words:
    list_words = text_noPunct.strip().split()

    # Convert to lower-case letters:
    list_words = [ word.lower() for word in list_words ]
    
    return list_words


def count_word_times(wordList):
    """
    Counts the number of times each word in wordList occurs.
    """
    wordDict = {}
    for word in wordList:
        if word in wordDict: #If the word already exists, increment the count.
            wordDict[word] +=1
        else: #If the word does not exist, add this word to the dictionary.
            wordDict[word] = 1
    return wordDict
    
    
#Take the word list
wordList = words_of_book()
#Counts the number of times each word occurs.
wordDict = count_word_times(wordList)
 
#Print the 100 most common words and the number of times each occur, in descending order.
print("Using dictionary")
n = 0
for word in sorted(wordDict, key=wordDict.get, reverse=True):
    print("[", n, "] ", word, " : ", wordDict[word], sep="")
    n += 1
    if n == 100:
        break

#Find a special python library or package to do this task with quickly.
print("\nUsing Counter in collections")
from collections import Counter
print(Counter(wordList).most_common(100))


