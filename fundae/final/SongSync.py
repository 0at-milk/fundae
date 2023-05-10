import serial
import random
from random import randrange
from operator import itemgetter
import datetime
import spotipy
from spotipy.oauth2 import SpotifyClientCredentials

import webbrowser

arduinoData = serial.Serial('/dev/cu.wchusbserial54D80019571', 921600)

'''
spotify API
'''
username = 'johanarivera81'
clientID = '4c5cb3def4de4d929a89218d7e23fcb9'
clientSecret = 'fd3d5745cac74e22a591a15f708f526b'
redirect_uri = 'http://localhost:5173/callback'


#Authentication - without user
client_credentials_manager = SpotifyClientCredentials(client_id=clientID, client_secret=clientSecret)
sp = spotipy.Spotify(client_credentials_manager = client_credentials_manager)

playlist_link = "https://open.spotify.com/playlist/5GIaihIJ7Ze4vIsk0E6usQ?si=a45a4c5c5b84434b"
playlist_URI = playlist_link.split("/")[-1].split("?")[0]
track_uris = [x["track"]["uri"] for x in sp.playlist_tracks(playlist_URI)["items"]]

'''
this function obtains a playlist -->track -->track audio info
it then sends this information to arduino to turn on the LED lights
'''
def getTrack():
    
    #using random choice to select a song from the playlist that hasn't been played before
    item = random.choice(tuple(sp.playlist_tracks(playlist_URI)["items"]))

    i = randrange(len(sp.playlist_tracks(playlist_URI)["items"]))
    item = sp.playlist_tracks(playlist_URI)["items"][i]
    item_uri = item["track"]["uri"]

    # Select item using index number
    audio_feats = sp.audio_features(item_uri)
    tempo_feature= list(map(itemgetter('tempo'), audio_feats))
    energy_feature=list(map(itemgetter('energy'), audio_feats))
    duration = list(map(itemgetter('duration_ms'), audio_feats))
    track_info = [item["track"]["name"], int(tempo_feature[0]), energy_feature[0]]
    
    print(item["track"]["name"])
    print("tempo ", tempo_feature[0])

    webbrowser.open(item_uri)
    
    sendTrack(track_info)
    startTimer(duration[0])

def sendTrack(trackInfo):
    listToStr = ', '.join([str(elem) for elem in trackInfo])
    listToStr = '<' + listToStr +'>'
    print("modified list is: ", listToStr)
    #this while loop must be true, or the input wont send
    cmd = listToStr +'\r'
    arduinoData.write(cmd.encode())

'''
This function repeats a song from the playlist after the current one is over
'''
def startTimer(duration):
    print("duration in micrseconds is: ", duration)
    i = 0
    start = datetime.datetime.now()
    print(start)
    last = start
    while True:
        now = datetime.datetime.now()
        # print('Elapsed: ' + str(now-start) + ' | Iteration #' + str(i))
        i += 1
        if now - last > datetime.timedelta(milliseconds=duration):
            last = now
            print('Elapsed: ' + str(now-start) + ' | Iteration #' + str(i))
            getTrack()
            break

getTrack()
