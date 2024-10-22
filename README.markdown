Licensed by AT&T under 'Software Development Kit Tools Agreement' 2012.
TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION: http://developer.att.com/sdk_agreement/
Copyright 2012 AT&T Intellectual Property. All rights reserved. 
For more information contact developer.support@att.com http://developer.att.com

# SimpleSpeech example app for AT&T Speech SDK

This sample app includes source code and an Xcode project to show how to call the AT&T Speech SDK from an application.  The app displays a simple client-side mashup.  Pressing a button initiates a speech interaction, a text area shows the recognition result, and a web view uses the recognized speech to search a website.

## Setting up the project

Done

## Running the sample

Done for configuration for your Speech API account: the URL of the service, your application's API key and API secret.  Set those values in `SpeechConfig.m`.   Before distributing your app to the public, make sure you add code to obfuscate those credentials.

## Understanding the speech code

The main code of the sample app is in the `SimpleSpeechViewController` class.  Look in the `-[prepareSpeech]` and `-[listen:]` methods for examples of setting up and starting a speech interaction.  Look in the `-[speechServiceSucceeded:]` and `[speechService:failedWithError:]` methods for examples of handling recognition responses.  

## Reusable OAuth code

The SpeechAuth class provides example code for authenticating your application with the OAuth client credentials protocol.  It performs an asynchronous network request for an OAuth access token that can be used in the Speech API.  Look in `-[SimpleSpeechViewController prepareSpeech]` for examples of calling SpeechAuth to obtain an access token. 
