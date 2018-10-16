#!/usr/bin/env python
# -*- coding: utf-8 -*-

# jarvis.py
# [cchao]

import websocket
import pickle
import json
import urllib
import requests
import sqlite3
import sklearn # you can import other sklearn stuff too!
# FILL IN ANY OTHER SKLEARN IMPORTS ONLY
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.naive_bayes import MultinomialNB
from sklearn.pipeline import Pipeline
from sklearn.linear_model import SGDClassifier
from sklearn import metrics
from sklearn.externals import joblib


import botsettings # local .py, do not share!!
TOKEN = botsettings.API_TOKEN
DEBUG = True

def debug_print(*args):
    if DEBUG:
        print(*args)


try:
    conn = sqlite3.connect("jarvis.db")
except:
    debug_print("Can't connect to sqlite3 database...")


def post_message(message_text, channel_id):
    requests.post("https://slack.com/api/chat.postMessage?token={}&channel={}&text={}&as_user=true".format(TOKEN,channel_id,message_text))


class Jarvis():
    def __init__(self): # initialize Jarvis
        self.JARVIS_MODE = None
        self.ACTION_NAME = None
        
        # SKLEARN STUFF HERE:
        #self.BRAIN = None # FILL THIS IN
        
        #naïve Bayes classifier
        self.BRAIN = Pipeline([('vect', CountVectorizer()), ('tfidf', TfidfTransformer()),('clf', MultinomialNB())])
        
        # support vector machine (SVM)
        #self.BRAIN = Pipeline([('vect', CountVectorizer()),('tfidf', TfidfTransformer()),('clf', SGDClassifier(loss='hinge', penalty='l2',alpha=1e-3, random_state=42, max_iter=5, tol=None))])

    """
    def classifier(self, channelID):    
        #Practice the tutorial and evaluate the performance of two classifiers
        
        ######Loading the dataset
        target_names = ['TIME', 'PIZZA', 'GREET', 'WEATHER', 'JOKE']
        data = []
        target = []   
        c = conn.cursor()
        #Get data from select statement
        for row in c.execute("SELECT * from training_data"):
            data.append(row[1])
            target.append(target_names.index(row[2]))
    
        ######Extracting features from text files                  
        ###Tokenizing text with scikit-learn
        count_vect = CountVectorizer()
        X_counts = count_vect.fit_transform(document for document in data)
        ###From occurrences to frequencies
        tfidf_transformer = TfidfTransformer()
        X_tfidf = tfidf_transformer.fit_transform(X_counts)
              
        ######Training a classifier
        clf = MultinomialNB().fit(X_tfidf, target)
        
        docs_new = ['Who wants pizzas?', 'What is the current time?']
        X_new_counts = count_vect.transform(docs_new)
        X_new_tfidf = tfidf_transformer.transform(X_new_counts)
        predicted = clf.predict(X_new_tfidf)

        for doc, category in zip(docs_new, predicted):
            print('%r => %s' % (doc, target_names[category]))
        
        ###Building a pipeline
        text_clf = Pipeline([('vect', CountVectorizer()), ('tfidf', TfidfTransformer()),('clf', MultinomialNB())])
        text_clf.fit(data, target)  
        
        for i in range(len(data)):
            if text_clf.predict([data[i]]) != target[i]:
                print("==========")
                print(data[i])
                print(text_clf.predict([data[i]]))
                print("==========")
        
        
        ###Evaluation of the performance on the test set
        import numpy as np
        #naïve Bayes classifier
        docs_test = data
        predicted = text_clf.predict(data)
        print(np.mean(predicted == target) )
        #post_message('{}'.format(np.mean(predicted == target) ), channelID)
        print(metrics.classification_report(target, predicted, target_names=[targetName for targetName in target_names]))
              
        # support vector machine (SVM)
        text_clf = Pipeline([('vect', CountVectorizer()),('tfidf', TfidfTransformer()),('clf', SGDClassifier(loss='hinge', penalty='l2',alpha=1e-3, random_state=42, max_iter=5, tol=None))])
        text_clf.fit(data, target)  
        predicted = text_clf.predict(docs_test)
        print(np.mean(predicted == target))    
        #post_message('{}'.format(np.mean(predicted == target) ), channelID)

        print(metrics.classification_report(target, predicted, target_names=[targetName for targetName in target_names]))                                         

        pass
    """ 

    def insert_db(self, action, msg_txt):
        """
        Insert training data to the database.
        """
        c = conn.cursor()     
        c.execute("INSERT INTO training_data (txt,action) VALUES (?, ?)", (msg_txt, action))       
        conn.commit() # save (commit) the changes       
        pass

    def find_target(self, msg_txt):
        """
        Find the action of the text
        """
        target_names = ['TIME', 'PIZZA', 'GREET', 'WEATHER', 'JOKE']
        data = []
        target = []   
        c = conn.cursor()

        for row in c.execute("SELECT * from training_data"):
            data.append(row[1])
            target.append(target_names.index(row[2]))         
            #print('{:3d}'.format(row[0])+row[2].rjust(9)+'  '+row[1])
                                         
        self.BRAIN.fit(data, target) 
        predicted = self.BRAIN.predict([msg_txt])
        
        return target_names[predicted[0]]
     
    """   
    def evaluate(self):
        target_names = ['TIME', 'PIZZA', 'GREET', 'WEATHER', 'JOKE']
        testData = ['What day is it today?','When will you start testing?','Am I late now?','How long have you been testing?','What is the date today?',
                'Grab me a pizza','Give me food','I want to order a whole chicken pizza with extra cheese','What kind of pizza do you have?','Bring me food with a lot of cheese',
                'How are you today?','Have a good day?','How’s everything going?','Glad to meet you','It\'s great seeing you.',
                'Is today a snow day?','Is it raining? Sunny? Or snowing?','What is the average temperature today?','Is today cold enough to wear a scarf?','Is it possible that it\'s raining outside?',
                'Tell me something interesting','Make me laugh with some jokes','Is there any joke for me?','A joke will break the silence now','Funny jokes are needed']
        correctTarget = [0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,4]
        
        correct = 0
        for test in range(len(testData)):
            result = self.find_target(testData[test])
            if result == target_names[correctTarget[test]]:
                correct+=1
            else:
                print(testData[test], end='')
                print("=>", end='')
                print(result)
        
        print(correct/len(testData))
            
        pass
    """
     
    def on_message(self, ws, message):
        m = json.loads(message)
        debug_print(m, self.JARVIS_MODE, self.ACTION_NAME)
        
        # only react to Slack "messages" not from bots (me):
        if m['type'] == 'message' and 'bot_id' not in m:
            channelID = m['channel']
            
            #self.classifier(channelID)
            #self.evaluate()
            
            if self.JARVIS_MODE == None:
                if m['text'].lower().strip() == 'training time':
                    post_message('OK, I\'m ready for training. What NAME should this ACTION be?', channelID)
                    self.JARVIS_MODE = 'training'
                elif m['text'].lower().strip() == 'testing time':
                    post_message('OK, I\'m ready for testing. Write me something and I\'ll try to figure it out.', channelID)
                    self.JARVIS_MODE = 'testing'
                elif m['text'].lower().strip() == 'done':
                    post_message('I\'m not doing nothing now.', channelID)
                else:
                    post_message('I can only recognize \'training time\', \'testing time\', and \'done\' when I\'m not in training mode and testing mode.', channelID) 
                
            elif self.JARVIS_MODE == 'training':
                if m['text'].lower().strip() == 'training time':
                    post_message('I\'m already in training mode!', channelID)
                    post_message('Now give me some training text!', channelID)
                elif m['text'].lower().strip() == 'testing time':
                    post_message('I\'m training my brain with the data you\'ve already given me...', channelID)
                    post_message('If you want to test me, type \'done\' first and type \'testing time\' second.', channelID)
                    post_message('Now give me some training text!', channelID)
                elif m['text'].lower().strip() == 'done':
                    post_message('OK, I\'m finished training', channelID)
                    self.JARVIS_MODE = None
                    self.ACTION_NAME = None
                    
                    #“pickled” copy of your final, trained classifier
                    joblib.dump(self.BRAIN, 'jarvis_brain.pkl') 
                    #clf = joblib.load('jarvis_brain.pkl') 
                else:
                    if self.ACTION_NAME == None:
                        self.ACTION_NAME = m['text'].upper().strip()
                        post_message('OK, let\'s call this action {}. Now give me some training text!'.format(self.ACTION_NAME), channelID)
                    else:
                        #Insert training data to the database
                        self.insert_db(self.ACTION_NAME, m['text'])
                        post_message('OK, I\'ve got it! What else?', channelID)
                      
            elif self.JARVIS_MODE == 'testing':
                if m['text'].lower().strip() == 'training time':
                    post_message('I\'m waiting for your text and testing if I can figure it out...', channelID)
                    post_message('If you want to train me, type \'done\' first and type \'training time\' second.', channelID)
                    post_message('Write me something and I\'ll try to figure it out.', channelID)
                elif m['text'].lower().strip() == 'testing time':
                    post_message('I\'m already in testing mode!', channelID)
                    post_message('Write me something and I\'ll try to figure it out.', channelID)
                elif m['text'].lower().strip() == 'done':
                    post_message('OK, I\'m finished testing', channelID)
                    self.JARVIS_MODE = None
                else:
                    #Find the action of the text
                    result = self.find_target(m['text'])
                    post_message('OK, I think the action you mean is {}'.format(result), channelID)
                    post_message('Write me something else and I\'ll try to figure it out.', channelID)
                    
            pass
        

def start_rtm():
    """Connect to Slack and initiate websocket handshake"""
    r = requests.get("https://slack.com/api/rtm.start?token={}".format(TOKEN), verify=False)
    r = r.json()
    r = r["url"]
    return r


def on_error(ws, error):
    print("SOME ERROR HAS HAPPENED", error)


def on_close(ws):
    conn.close()
    print("Web and Database connections closed")


def on_open(ws):
    print("Connection Started - Ready to have fun on Slack!")


r = start_rtm()
jarvis = Jarvis()
ws = websocket.WebSocketApp(r, on_message=jarvis.on_message, on_error=on_error, on_close=on_close)
ws.run_forever()

