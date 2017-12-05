# coding: utf-8

import praw

reddit = praw.Reddit(client_id='8pTH5EvuT6BREQ',
                     client_secret='BQSAlS3ei8GMhfL04su4RfCQrSs',
                     password='CMPE130bot',
                     user_agent='testscript by /u/TinyPooter',
                     username='TinyPooter')

f = open('Comments.txt','w')

user = reddit.redditor('nowwhathappenedwas')

for comment in user.comments.new(limit=None):
    print("|||", file = f)
    print(comment.subreddit, file = f)
    print(comment.score, file = f)
    print(comment.body, file = f)

f.close()
