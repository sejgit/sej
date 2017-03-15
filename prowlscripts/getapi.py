#!/usr/bin/env python
# -*- coding: utf-8 -*-

### get users api key

"""
get users api key
"""
import prowlpy

# get a api_key and provider key at https://www.prowlapp.com/api_settings.php
api_key = raw_input('Api Key: ')
provider_key = raw_input('Provider Key (optional): ')

p = prowlpy.Prowl(api_key)

# get token for user
response = p.retrieve_token(provider_key)

print "I got token %s for %s" % (response['token'], response['url'])

# assuming that the user has accepted your request
# (they go to the response["url"] via a browser to accept)
# retrieve his/her api_key
raw_input("Press Enter to continue...")
users_api_key = p.retrieve_apikey(provider_key, response["token"])
print "users api key is: %s" % (users_api_key['apikey'])

print "Done."
