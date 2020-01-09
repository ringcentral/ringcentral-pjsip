say -o greetings_temp.wav --data-format=LEI16@48000 "Welcome to the telephony survey demo" && ffmpeg -i greetings_temp.wav audios/greetings.wav && rm greetings_temp.wav
say -o question_temp.wav --data-format=LEI16@48000 "Which color do you like? 1 for red, 2 for green, 3 for blue" && ffmpeg -i question_temp.wav audios/question.wav && rm question_temp.wav
say -o invalid_temp.wav --data-format=LEI16@48000 "Invalid option" && ffmpeg -i invalid_temp.wav audios/invalid.wav && rm invalid_temp.wav
say -o red_temp.wav --data-format=LEI16@48000 "You like red" && ffmpeg -i red_temp.wav audios/red.wav && rm red_temp.wav
say -o green_temp.wav --data-format=LEI16@48000 "You like green" && ffmpeg -i green_temp.wav audios/green.wav && rm green_temp.wav
say -o blue_temp.wav --data-format=LEI16@48000 "You like blue" && ffmpeg -i blue_temp.wav audios/blue.wav && rm blue_temp.wav
say -o bye_temp.wav --data-format=LEI16@48000 "The demo is over, the call will be hang up in 3 seconds, bye" && ffmpeg -i bye_temp.wav audios/bye.wav && rm bye_temp.wav
