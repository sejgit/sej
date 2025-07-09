#!/usr/bin/env python3
"Script to get access code from local Yolink hub."

import sys
import os.path
import requests
import logging
from typing import Self
import json

LOGGER = logging.getLogger(__name__)

class Yolink:
    def __init__(self, url, id, secret) -> None:
        self.local_URL = url
        self.local_client_id = id
        self.local_client_secret = secret
        self.local_access_key = ""

    def yolinkAccessKey(self) -> Self:
        try:
            response = requests.post(self.local_URL+'/open/yolink/token',
                data={ "grant_type": "client_credentials",
                       "client_id" :  self.local_client_id ,
                       "client_secret":self.local_client_secret },
                timeout=10)
            if response.ok:
                self.local_access_key = response.json()
                LOGGER.debug('Local yoAccess Token : {}'.format(self.local_access_key))
        except Exception as err:
            LOGGER.error(f"other error: {err}")
        return self

if __name__ == "__main__":
    try:
        with open(os.path.expanduser("~/.ssh/yolink-local-api.json"), "r") as jsonfile:
            data = json.load(jsonfile)
        print(data)
        url = data["local-url"]
        id = data["local-client-id"]
        secret = data["local-client-secret"]
        key = Yolink(url, id, secret)
        key.yolinkAccessKey()
    except (KeyboardInterrupt, SystemExit):
        LOGGER.warning("Received interrupt or exit...")
        """
        Catch SIGTERM or Control-C and exit cleanly.
        """
    except Exception as err:
        LOGGER.error(f"Error: {err}")
    sys.exit(0)



