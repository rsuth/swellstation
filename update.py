import requests
import re
import math
import os
from dotenv import load_dotenv

def get_raw_data(url):
    try:
        response = requests.get(url)
        return response.text
    except requests.exceptions.RequestException:
        return 'Cannot Get Data'

def parse_raw_data(raw_data):
    pattern = r'(?:20\d{2})(?:\s{1,3}\S{2}){7}\s+(?P<wave_height>\S+)\s+(?P<dom_period>\S+)\s+\S+\s+(?P<degrees>\d{3})'
    m = re.search(pattern, raw_data)
    
    if m:
        return m.groupdict()
    else:
        return 'Error Parsing Data'

def build_data_string(parsed_data):
    wave_ft = round(float(parsed_data['wave_height'])*3.28, 1)
    wave_period = parsed_data['dom_period']

    # add padding zeros if needed
    if(len(f"{wave_ft}") < 4):
        wave_ft = f"0{wave_ft}"
    if(len(f"{wave_period}") < 2):
        wave_period = f"0{wave_period}"

    degrees = int(parsed_data['degrees'])
    cardinal = degrees_to_cardinal(degrees)
    return f"{wave_ft}{wave_period}{degrees}{cardinal}"

# from https://gist.github.com/RobertSudwarts/acf8df23a16afdb5837f
def degrees_to_cardinal(d):
    '''
    note: this is highly approximate...
    '''
    dirs = ["  N", "NNE", " NE", "ENE", "  E", "ESE", " SE", "SSE",
            "  S", "SSW", " SW", "WSW", "  W", "WNW", " NW", "NNW"]
    ix = int((d + 11.25)/22.5)
    return dirs[ix % 16]

# function sends an http request to the particle.io cloud with my photon's id 
# and access token.
def send_to_station(photon_device_id, photon_access_token, data):
    r = requests.post(f'https://api.particle.io/v1/devices/{photon_device_id}/update', data={
        "access_token": photon_access_token,
        "args": data
    })
    return r.content

load_dotenv(".env")

# get configuration variables from environment
device_id = os.getenv('PHOTON_ID')
access_token = os.getenv('PHOTON_ACCESS_TOKEN')
buoy_id = os.getenv('BUOY_ID')

# id from the list here: https://www.ndbc.noaa.gov/
buoy_url = f'https://www.ndbc.noaa.gov/data/realtime2/{buoy_id}.txt'

raw_data = get_raw_data(buoy_url)
parsed_data = parse_raw_data(raw_data)
print(parsed_data)
datastring = build_data_string(parsed_data)


print(datastring)
resp = send_to_station(device_id, access_token, datastring)
print(resp)