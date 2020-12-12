# Init Lab - Grader

import sys
import subprocess
import json
from yaml import load, Loader
from difflib import SequenceMatcher
import math

PRINT_ORDER = ['q1', 'q2']

SCORES = {
    'q1': {
        'description': 'Question sur les tubes nommes',
        'parametres des programmes (programme ecrivant dans le tube b)': {
            'description': 'Les programmes sont lances avec les bons pids',
            'score': 0,
            'scale': 1.5,
        },
        'parametres des programmes (programme ecrivant dans le tube a)': {
            'description': 'Les programmes sont lances avec les bons pids',
            'score': 0,
            'scale': 1.5,
        },
        'echange avec tubes nommes (le programme ecrivant dans a recoit le message venant de b)': {
            'description': 'La communication est etablie correctement',
            'score': 0,
            'scale': 2.5,
        },
        'echange avec tubes nommes (le programme ecrivant dans b recoit le message venant de a)': {
            'description': 'La communication est etablie correctement',
            'score': 0,
            'scale': 2.5,
        }
    },
    'q2': {
        'description': 'Question sur la ligne de commande bash',
        'reproduction de la ligne de commande (prog1)': {
            'description': 'La ligne de commande est correcte',
            'scale': 2,
            'score': 0
        },
        'reproduction de la ligne de commande (prog2)': {
            'description': 'La ligne de commande est correcte',
            'scale': 3,
            'score': 0
        },
        'reproduction de la ligne de commande (prog3)': {
            'description': 'La ligne de commande est correcte',
            'scale': 3,
            'score': 0
        }
    }
}

PARAMS = {}
REPORT_STATUS = {
    'ignore': False
}

def handleReportLine(reportLine):


    evaluated = [False, False, False, False, False, False, False]

    # Little trick here to ignore second execution of "./testlab 2" in the grading script OLD
    if REPORT_STATUS['ignore']:
        return
    if reportLine == 'ENDREPORT':
        REPORT_STATUS['ignore'] = True
        return
    
    if reportLine == "q1: pidprogbOK" and not evaluated[0]:
        evaluated[0] = True
        SCORES['q1']['parametres des programmes (programme ecrivant dans le tube b)']['score'] = 1.5
        return

    if reportLine == "q1: pidprogaOK" and not evaluated[1]:
        evaluated[1] = True
        SCORES['q1']['parametres des programmes (programme ecrivant dans le tube a)']['score'] = 1.5
        return
    
    if reportLine == "q1: progaok" and not evaluated[2]:
        evaluated[2] = True
        SCORES['q1']['echange avec tubes nommes (le programme ecrivant dans a recoit le message venant de b)']['score'] = 2.5
        return

    if reportLine == "q1: progbok" and not evaluated[3]:
        evaluated[3] = True
        SCORES['q1']['echange avec tubes nommes (le programme ecrivant dans b recoit le message venant de a)']['score'] = 2.5
        return

    if reportLine == "q2: prog1ok--2" and not evaluated[4]:
        SCORES["q2"]["reproduction de la ligne de commande (prog1)"]["score"] = 2
        evaluated[4] = True
        return

    if reportLine == "q2: prog2ok--3" and not evaluated[5]:
        SCORES["q2"]["reproduction de la ligne de commande (prog2)"]["score"] = 3
        evaluated[5] = True
        return

    if reportLine == "q2: prog3ok--3" and not evaluated[6]:
        SCORES["q2"]["reproduction de la ligne de commande (prog3)"]["score"] = 3
        evaluated[6] = True
        return

    # key, token = reportLine.split(": ")

    # if 'countEvents' in SCORES[key][token]:
    #     SCORES[key][token]['countEvents'] = SCORES[key][token]['countEvents'] + 1
    # elif 'granted' in SCORES[key][token]:
    #     SCORES[key][token]['granted'] = True
    # elif 'unitScale' in SCORES[key][token]:
    #     SCORES[key][token]['score'] = SCORES[key][token]['score'] + SCORES[key][token]['unitScale']


def grade(gradeLogPath, paramsFilePath, mode):
    handoutMode = True if mode == 'handout' else False
    # OK lines are working
    with open(paramsFilePath, 'r', encoding='utf-8') as paramsFile:
        paramsAsStr = paramsFile.read()
        for key, obj in load(paramsAsStr, Loader=Loader).items():
            PARAMS[key] = obj

    with open(gradeLogPath, "r", errors="ignore") as gradeLogFile:
        lines = gradeLogFile.readlines()

    for l in lines:
        handleReportLine(l.rstrip())

    print("--------------------")
    print("Redirections Lab: Autograder")
    print("--------------------")
    print("")

    totalScore = 0
    totalScale = 0
    bonusScore = 0
    bonusScale = 0
    scoresJSON = {
        'scores': {}
    }
    
    for problem in PRINT_ORDER:
        obj = SCORES[problem]
        print(obj['description'])
        print("------------------------------------------------------------------------------")
        
        totalProblemScore = 0
        totalProblemScale = 0
        bonusProblemScore = 0
        bonusProblemScale = 0
        problemInvalidated = False
        for key, subscores in obj.items():
            if key  == 'description':
                continue
            
            if 'granted' in subscores:
                subscore = subscores['scale'] if subscores['granted'] else 0.0
            elif 'score' in subscores:
                # Round score
                subscores['score'] = round(subscores['score'] * 100) / 100
                subscore = subscores['score']
            elif 'passed' in subscores:
                if not subscores['passed']:
                    problemInvalidated = True
            else:
                subscore = 0
            if 'passed' in subscores:
                if subscores['passed']:
                    passedStr = 'PASSED'
                else:
                    passedStr = 'FAILED'
                row = ['-> ' + subscores['description'], str(passedStr)]
                print("{: <65} {: >11}".format(*row))
            else:
                if 'isBonus' in subscores and subscores['isBonus']:
                    row = ['-> ' + subscores['description'], "+" + str(subscore), "/" + str(subscores['scale'])]
                    print("{: <65} {: >5} {: >5}".format(*row))
                else:
                    row = ['-> ' + subscores['description'], str(subscore), "/" + str(subscores['scale'])]
                    print("{: <65} {: >5} {: >5}".format(*row))

            if not 'passed' in subscores:
                if 'isBonus' in subscores and subscores['isBonus']:
                    bonusProblemScore = bonusProblemScore + subscore
                    bonusProblemScale = bonusProblemScale + subscores['scale']
                else:
                    totalProblemScore = totalProblemScore + subscore
                    totalProblemScale = totalProblemScale + subscores['scale']
        
        # Round score
        totalProblemScore = round(totalProblemScore * 100) / 100
        totalProblemScale = round(totalProblemScale * 100) / 100

        if problemInvalidated:
            totalProblemScore = 0
            bonusProblemScore = 0
            if totalProblemScale > 0:
                row = ['TOTAL', 'FAILED']
                print("{: <65} {: >11}".format(*row))
            if bonusProblemScale > 0:
                row = ['BONUS', 'FAILED']
                print("{: <65} {: >11}".format(*row))
        else:
            if totalProblemScale > 0:
                row = ['TOTAL', str(totalProblemScore), "/" + str(totalProblemScale)]
                print("{: <65} {: >5} {: >5}".format(*row))
            if bonusProblemScale > 0:
                row = ['BONUS', "+" + str(bonusProblemScore), "/" + str(bonusProblemScale)]
                print("{: <65} {: >5} {: >5}".format(*row))
        print("")

        totalScore = totalScore + totalProblemScore
        totalScale = totalScale + totalProblemScale
        bonusScore = bonusScore + bonusProblemScore
        bonusScale = bonusScale + bonusProblemScale
        scoresJSON['scores'][problem] = totalProblemScore + bonusProblemScore
    
    print("------------------------------------------------------------------------------")
    row = ['Total score', str(totalScore), "/" + str(totalScale)]
    print("{: <65} {: >5} {: >5}".format(*row))
    if bonusScale > 0:
        row = ['Bonus', "+" + str(bonusScore), "/" + str(bonusScale)]
        print("{: <65} {: >5} {: >5}".format(*row))
        row = ['TOTAL', str(bonusScore + totalScore), "/" + str(totalScale)]
        print("{: <65} {: >5} {: >5}".format(*row))
        
    print("------------------------------------------------------------------------------")
    if not handoutMode:
        print(json.dumps(scoresJSON))

if __name__ == '__main__':
    grade(sys.argv[1], sys.argv[2], sys.argv[3])
