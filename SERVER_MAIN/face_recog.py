import numpy as np
import cv2
import face_recognition as fr
from glob import glob
import pickle
import utility
import random
import define_constants as const
import urllib.request
import time
from test import test



def checkImage():
    # Load data from pickle file (n_people)
    with open('assets/pickles/n_people.pk', 'rb') as pickle_file:
        n_people_in_pickle = pickle.load(pickle_file)
    print(f"Number of files that should be in '{const.PEOPLE_DIR}' directory : {n_people_in_pickle}")

    # Read all images
    people = glob(const.PEOPLE_DIR + '/*.*')
    print(f"Number of files in '{const.PEOPLE_DIR}' directory : {len(people)}")
    if n_people_in_pickle!=len(people):
        print(f"Number of files mismatch in pickle and picture folder")
    isIdentified = False

    # Check if number of files in PEOPLE_DIR is same as value in pickle file
    if n_people_in_pickle == len(people):
        # Get names
        names = list(map(utility.get_names, people))

        # Get encodings
        face_encode = np.load('assets/face_encodings/data.npy')

        # Initiate Webcam
        print("\nInitiating camera...\n")
        cap = cv2.VideoCapture(const.n_camera, cv2.CAP_DSHOW)

        # Constants for eye blink detection
        eye_blink_total = 0
        frame_current_name = None
        isEyeClosed = False

        err = 0
        start_time = time.time()
        while True:
            if time.time()-start_time>30:
                cap.release()
                cv2.destroyAllWindows()
                return False
            # Read Frames
            ret, frame = cap.read()

            lab = test(image=frame,model_dir='E:\\cse offline 3-1\\pythonProject\\Silent_Face_Anti_Spoofing\\resources\\anti_spoof_models',device_id=0)

            # print(lab)
            if lab==2:
                err=err+1
            else:
                err=0
            if err==5:
                print("spoffing!!")
                return False
            frame_face_loc = fr.face_locations(frame)
            frame_face_landmarks = fr.face_landmarks(frame, frame_face_loc)
            frame_face_encode = fr.face_encodings(frame, frame_face_loc)

            did_for_loop_run = False
            # Iterate through locations, landmarks and encodings
            for index, (loc, encode, landmark) in enumerate(
                    zip(frame_face_loc, frame_face_encode, frame_face_landmarks)):
                did_for_loop_run = True
                # Find index match
                # is_face_same = fr.compare_faces(face_encode, encode)
                score = fr.face_distance(face_encode, encode)
                index_match = np.argmin(score)

                # Check if min(score) is < face_recognition_threshold
                if np.min(score) < const.face_recognition_threshold:
                    # Store name temporarily to check if frame_current_name matches with temp_name
                    temp_name = frame_current_name
                    # Store new name
                    frame_current_name = names[index_match]
                else:
                    frame_current_name = "Unknown"

                # If frame_current_name is Unknown don't detect eye (and record attendance)
                if not frame_current_name == "Unknown":
                    # Eye blink detection
                    left_eye_points = np.array(landmark['left_eye'], dtype=np.int32)
                    right_eye_points = np.array(landmark['right_eye'], dtype=np.int32)

                    # EAR_left = get_EAR_ratio(left_eye)
                    # EAR_right = get_EAR_ratio(right_eye)
                    EAR_avg = (utility.get_EAR_ratio(left_eye_points) + utility.get_EAR_ratio(right_eye_points)) / 2

                    # Check if EAR ratio is less than threshold
                    if EAR_avg < const.EAR_ratio_threshold:
                        isEyeClosed = True
                    elif EAR_avg >= const.EAR_ratio_threshold and isEyeClosed:
                        eye_blink_total +=1
                        isEyeClosed = False
                    if temp_name != frame_current_name:
                        eye_blink_total = 0
                        isEyeClosed = False

                    blink_message = f"Blink 2 times, blinks:{eye_blink_total}"
                    face_box_color = const.default_face_box_color

                    if 2 == eye_blink_total:

                        if np.min(score) < const.face_recognition_threshold:
                            utility.record_log(frame_current_name)
                            face_box_color = const.success_face_box_color  # Set face box color to green for one frame
                            eye_blink_total = 0
                            isIdentified = True

                    cv2.polylines(frame, [left_eye_points], True, const.eye_color, 1)
                    cv2.polylines(frame, [right_eye_points], True, const.eye_color, 1)
                    cv2.putText(frame, blink_message, (10, 50), cv2.FONT_HERSHEY_PLAIN, 1.5, const.text_in_frame_color,2)
                    cv2.putText(frame, "Only one person at a time", (10, 30), cv2.FONT_HERSHEY_PLAIN, 1.5, const.text_in_frame_color,2)
                else:
                    eye_blink_total = 0
                    isEyeClosed = False
                    face_box_color = const.unknown_face_box_color

                cv2.rectangle(frame, (loc[3], loc[0]), (loc[1], loc[2]), face_box_color, 2)  # top-right, bottom-left
                cv2.putText(frame, frame_current_name, (loc[3], loc[0] - 3), cv2.FONT_HERSHEY_PLAIN, 2,
                            const.text_in_frame_color, 2)

            if not did_for_loop_run:
                eye_blink_total = 0
                isEyeClosed = False
            if isIdentified:
                isIdentified = False
                cv2.putText(frame, 'Your are identified', (20, 450), cv2.FONT_HERSHEY_PLAIN, 2,
                            const.text_in_frame_color, 2)
                # Display frame
                cv2.imshow("You are recognized", frame)

                cap.release()
                cv2.destroyAllWindows()
                return True
            else:
                cv2.imshow("Blink and identify yourself again", frame)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                cap.release()
                cv2.destroyAllWindows()
                return False
    else:
        print(f"Run encode_faces.py to encode all faces in '{const.PEOPLE_DIR}' directory...")
print(checkImage())