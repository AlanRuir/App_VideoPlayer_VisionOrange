#!/bin/bash

INPUT_FILE="/home/nyx/Datas/Videos/AVM/avm-1920x760.mp4"
RTSP_SERVER_URL="rtsp://127.0.0.1:554/live/test"

if [ ! -f "$INPUT_FILE" ]; then
  echo "输入文件不存在: $INPUT_FILE"
  exit 1
fi

ffmpeg -re -stream_loop -1 -i "$INPUT_FILE" -c copy -f rtsp "$RTSP_SERVER_URL"