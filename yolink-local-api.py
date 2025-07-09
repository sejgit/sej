#!/usr/bin/env python3
"Script to get access code from local Yolink hub using file in .ssh directory."

import sys
import os.path
import requests
import logging
import json

LOGGER = logging.getLogger(__name__)

class Yolink:
    def __init__(self, data):
        self.URL = data["local-url"]
        self.client_id = data["local-client-id"]
        self.client_secret = data["local-client-secret"]
        self.access_key = ""

    def yolinkAccessKey(self):
        try:
            response = requests.post(self.URL+'/open/yolink/token',
                data={ "grant_type": "client_credentials",
                       "client_id" :  self.client_id ,
                       "client_secret":self.client_secret },
                timeout=10)
            if response.ok:
                self.access_key = response.json()
                LOGGER.debug('Local yoAccess Token : {}'.format(self.access_key))
        except Exception as err:
            LOGGER.error(f"other error: {err}")

if __name__ == "__main__":
    try:
        with open(os.path.expanduser("~/.ssh/yolink-local-api.json"), "r") as jsonfile:
            data = json.load(jsonfile)
        print(data)
        key = Yolink(data)
        key.yolinkAccessKey()
    except (KeyboardInterrupt, SystemExit):
        LOGGER.warning("Received interrupt or exit...")
    except Exception as err:
        LOGGER.error(f"Error: {err}")
    sys.exit(0)



