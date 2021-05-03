# Swell Station
### An internet connected oceanic conditions display
Richard Sutherland - CS530 Spring 2021

Swell station utilizes a [Particle Photon](https://docs.particle.io/photon/) internet connected microcontroller to display current oceanic conditions on 2 analog dials.

## Structure

The station consists of a client (Particle Photon) and a server (update.py).
swellstation.ino is the client source code. It is compiled and flashed to the 
microcontroller. It recieves data from the Particle cloud API and updates the analog dials.

update.py is the server. It can run on any computer. Ideally it will be set to run at
set intervals using cronjobs or the like. It gets the buoy data from NOAA, parses it,
and sends it to the Particle cloud API. Sensitive items like the API key and particular device ID
are stored as environment variables in a file called .env, which is ommitted for security reasons.
The buoy ID is also stored as an environment variable. It can be set to any NDBC ID, all of which
can be found [here](https://www.ndbc.noaa.gov/).

## How to Use:

Follow the procedures here to get your Particle Photon on wifi. Flash the swellstation.ino code to the photon. Then, on the server set your ENV variables in a file called .env with the following structure:

```
PHOTON_ID=<Your photon ID>
PHOTON_ACCESS_TOKEN=<Your photon access token>
BUOY_ID=<NOAA buoy ID>
```

Then, run update.py to update the swell station. Set it to run however often you like with a cron job or the like.
Generally, NOAA only refreshes thier data every 10 or so minutes, so any interval shorter than that is a waste.

