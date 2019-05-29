
FROM arm32v7/python:3.8.0a2-stretch

RUN pip install pyserial

RUN apt-get update

RUN pip install serial_device2

RUN pip install RPi.GPIO

COPY final.py ./final.py

CMD ["python","./final.py"]
