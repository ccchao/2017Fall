# STAT/CS 287
# HW 01 SOLUTION
#
# Name: James Bagrow
# Last modified: 2017-08-01

#####          Solution for Homework 01          ######


print("******************** HW01: Problem 1/3 ********************")

def similarity(A,B):
    """Return jaccard index between two sets A and B, defined as
    similarity(A,B) = |A intersection B| / |A union B|.
    """
    return len( A & B ) / len( A | B )


set1 = set(["1",7,"8",0,3])
set2 = set(range(10))
set3 = set([3,4,7,9,11,12])

print("similarity(set1,set2) =", similarity(set1,set2))
print("similarity(set1,set3) =", similarity(set1,set3))
print("similarity(set2,set3) =", similarity(set2,set3))




print("******************** HW01: Problem 2/3 ********************")

import random


def coin_flip(p):
    """Flip a biased coin. Return 'H' with prob. p and 'T' with probability
    1-p.
    """
    if random.random() < p:
        return 'H'
    return 'T'


def run_statistics(flips):
    """Given a list of 'H's and 'T's from a flipped coin, compute the number of
    times we observe 1,2,3,... consecutive heads.
    """
    
    # Add a T to the end of the flips list so we always catch the final run in
    # the below loop:
    flips.append("T")
    
    # Get the length of each run of contiguous values:
    curr_run = 0
    run_lengths = []
    for f in flips:
        if f == "H": # our run of heads continues
            curr_run += 1
        else:        # got a tails, terminate this run and record
            run_lengths.append( curr_run )
            curr_run = 0 # reset counter for next run
    
    # Now we need to count how many times we found a run of 1,2,3,... heads.
    # Use a dict to count the numbers of each run length:
    runlength2num = {}
    for run_len in run_lengths:
        try:
            runlength2num[ run_len ] += 1
        except KeyError: # first time we've seen a run of length `run_len`
            runlength2num[ run_len ] = 1
    
    # Runs of length zero (1 or more tails in a row) are boring, remove them:
    del runlength2num[0]
    
    return runlength2num


### BONUS ###
import collections

def run_statistics2(flips):
    """Another way to compute runs stats, relatively more readable that 2"""
    run_lengths = [ len(f) for f in "".join(flips).split("T") if f ]
    #run_lengths = []
    #for f in "".join(flips).split("T"):
    #    if f:
    #        run_lengths.append(len(f))
    return dict( collections.Counter(run_lengths) )
#############


for p in [0.2,0.4,0.6,0.8]:
    
    # Flip 1000 coins:
    coins = []
    for _ in range(1000):
        coins.append( coin_flip(p) )

    # Compute run statistics:
    runLen2numOcc  = run_statistics(coins) # dict mapping runlength to number of occurrences
    runLen2numOcc2 = run_statistics2(coins)
    print("Are both run statistics methods giving the same answer?", runLen2numOcc == runLen2numOcc2)

    # Print out the number of occurrences of each run length, sorted by run_len:
    for r,n in sorted( runLen2numOcc.items() ): # remember dict order is usually arbitrary
        print("r = %2i, n = %3i (p = %0.2f)" % (r,n,p))
    print()


# Analytic solution: p^L (1-p)^2


print("******************** HW01: Problem 3/3 ********************")

import urllib.request
from string import punctuation

def words_of_book():
    """Download `A tale of two cities` from Project Gutenberg if you haven't
    already. Return a list of words. Punctuation has been removed and
    upper-case letters have been replaced with lower-case.
    """

    # check local cache:
    fname = "taleTwoCities.txt"
    try:
        raw = open(fname).read()
    except IOError: # file doesn't exist, let's download it and cache it
        url = "http://www.gutenberg.org/files/98/98.txt"
        req = urllib.request.urlopen(url) # Download book into one long string.
        charset = req.headers.get_content_charset()
        raw = req.read().decode(charset)
        fout = open(fname, 'w')
        fout.write(raw)
        fout.close()
    # WARNING: Project Gutenberg may trigger a recaptcha. Be sure you've got
    # the text of the book and not the text of the web page asking you to
    # confirm you are not a robot.

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


# Call the given function to get the list of words:
list_words = words_of_book()

# Count how many times each word appears in the list:
word2count = {}
for word in list_words:
    try:
        word2count[word] += 1
    except KeyError:
        word2count[word] = 1
### ################################################################### ###
### MUCH FASTER than looping over the entire book for every unique word ###
### ################################################################### ###

# Now sort the words by their counts like we did in class:
count_word = sorted([ (word2count[w],w) for w in word2count ], reverse=True)

# Get the top 100 words and print them and their counts:
top_words = [ w for c,w in count_word ][:100]

for word in top_words:
    print(word, word2count[word])

    
"""
******************** HW01: Problem 1/3 ********************
similarity(set1,set2) = 0.25
similarity(set1,set3) = 0.2222222222222222
similarity(set2,set3) = 0.3333333333333333
******************** HW01: Problem 2/3 ********************
Are both run statistics methods giving the same answer? True
r =  1, n = 128 (p = 0.20)
r =  2, n =  37 (p = 0.20)
r =  3, n =   6 (p = 0.20)
r =  4, n =   1 (p = 0.20)
r =  5, n =   1 (p = 0.20)
r =  8, n =   1 (p = 0.20)

Are both run statistics methods giving the same answer? True
r =  1, n = 153 (p = 0.40)
r =  2, n =  50 (p = 0.40)
r =  3, n =  22 (p = 0.40)
r =  4, n =  10 (p = 0.40)
r =  5, n =   1 (p = 0.40)
r =  6, n =   1 (p = 0.40)
r =  7, n =   1 (p = 0.40)

Are both run statistics methods giving the same answer? True
r =  1, n =  88 (p = 0.60)
r =  2, n =  63 (p = 0.60)
r =  3, n =  46 (p = 0.60)
r =  4, n =  21 (p = 0.60)
r =  5, n =  15 (p = 0.60)
r =  6, n =  12 (p = 0.60)
r =  7, n =   2 (p = 0.60)
r =  8, n =   1 (p = 0.60)
r = 11, n =   1 (p = 0.60)

Are both run statistics methods giving the same answer? True
r =  1, n =  28 (p = 0.80)
r =  2, n =  22 (p = 0.80)
r =  3, n =  16 (p = 0.80)
r =  4, n =  14 (p = 0.80)
r =  5, n =   9 (p = 0.80)
r =  6, n =  15 (p = 0.80)
r =  7, n =  12 (p = 0.80)
r =  8, n =   8 (p = 0.80)
r =  9, n =  10 (p = 0.80)
r = 10, n =   3 (p = 0.80)
r = 11, n =   2 (p = 0.80)
r = 12, n =   3 (p = 0.80)
r = 13, n =   2 (p = 0.80)
r = 14, n =   2 (p = 0.80)
r = 15, n =   2 (p = 0.80)
r = 16, n =   1 (p = 0.80)
r = 17, n =   3 (p = 0.80)
r = 20, n =   1 (p = 0.80)

******************** HW01: Problem 3/3 ********************
the 8186
and 4993
of 4125
to 3543
a 2974
in 2638
it 2013
his 2005
i 1917
that 1904
he 1833
was 1764
you 1455
with 1351
had 1297
as 1148
at 1045
her 1038
for 972
him 965
on 932
not 860
is 842
be 780
have 742
said 660
were 658
but 654
my 653
mr 620
this 588
so 582
by 577
all 571
there 567
they 564
no 548
from 529
me 522
if 471
she 459
out 446
one 438
been 435
when 434
or 434
which 409
them 393
who 375
what 371
an 349
your 345
would 341
lorry 336
are 333
into 319
their 318
do 315
up 309
will 295
upon 291
could 282
defarge 280
man 279
little 265
more 261
its 261
any 261
time 260
now 256
then 253
hand 247
miss 232
down 232
before 232
know 230
some 229
again 227
am 225
himself 219
very 217
than 216
two 211
good 209
see 198
like 198
other 193
looked 193
never 192
long 192
doctor 192
madame 191
these 187
face 187
old 186
made 185
here 184
night 182
much 181
way 180
"""
