#!/usr/bin/python3

import os
import subprocess
import csv
import re
import math
from pathlib import Path

currentdir = os.getcwd()
logfilename = "log_results.txt"
pval_filename = "pval_results.txt"

DRN_OFFSET = 0.5
_5HTR2_OFFSET = 0.5

standard_food_delay = 15
shorter_food_delay = 10

uncertainty = 0.1

# visual input if landmark can be seen
vis1 = 1

# visual input if reward shows up
vis2 = 1

def ofc5HTreceptors(_5ht,x,t):
    serot = _5ht
    if t == 2:
        serot = _5ht + DRN_OFFSET
    SEROT_R2_OFFSET = 0
    if t == 3:
        SEROT_R2_OFFSET = _5HTR2_OFFSET
    return (1-math.exp(-pow(x/(serot+1),(serot+1))))*(serot+2+SEROT_R2_OFFSET)


def doExperiment(delay,reward_unseen_speed,reward_seen_speed,speed_uncertainty,reportfile,resultsfile):
    # running the script
    delay = int(delay)
    reward_unseen_speed = int(reward_unseen_speed)
    speed_uncertainty = int(speed_uncertainty)
    reward_seen_speed = int(reward_seen_speed)
    parameters = "delay={},reward_unseen_speed={},reward_seen_speed={},speed_uncertainty={}".format(delay,
                                                                                                    reward_unseen_speed,
                                                                                                    reward_seen_speed,
                                                                                                    speed_uncertainty)
    print("Running experiment with: "+parameters)
    reportfile.write("SCRIPT RUN:\n")
    try:
        result = "SUCCESS:\n"+ subprocess.check_output(["prism","final_5ht.prism","prop2_new.pctl",
                                                        "-const",parameters], stderr=subprocess.STDOUT).decode("ISO-8859-1")
    except subprocess.CalledProcessError as e:
        result = "CRASH!!:\n"+ e.output.decode("ISO-8859-1")
        resultsfile.write("NAN\n")
        return
    print(result)
    reportfile.write(result)
    reportfile.write("\n")
    print("\n")
    reportfile.write("\n\n\n\n")
    reportfile.flush()
    result_in_lines = result.splitlines()
    for line in result_in_lines:
        if 'Result:' in line:
            rr = re.findall("[-+]?[.]?[\d]+(?:,\d\d\d)*[\.]?\d*(?:[eE][-+]?\d+)?", line)
            rr = float(rr[0])
            resultsfile.write(str(rr)+"\n")
            resultsfile.flush()
            print("p = {}\n\n".format(rr))
            return
        

reportfile = open(logfilename, 'wt')
resultsfile = open(pval_filename, 'wt')

print("1 - Normal")
reward_unseen_speed = 100 * ofc5HTreceptors(3, vis1, 1)
reward_seen_speed = 100 * ofc5HTreceptors(3, vis1+vis2, 1)
doExperiment(standard_food_delay,
             reward_unseen_speed,
             reward_seen_speed,
             reward_unseen_speed * uncertainty,
             reportfile,resultsfile)

print("2 - Less delay")
doExperiment(shorter_food_delay,
             reward_unseen_speed,
             reward_seen_speed,
             reward_unseen_speed * uncertainty,
             reportfile,resultsfile)

print("3 - DRN is suppressed")
reward_unseen_speed = 100 * ofc5HTreceptors(0.3, vis1, 1)
reward_seen_speed = 100 * ofc5HTreceptors(0.3, vis1+vis2, 1)
doExperiment(standard_food_delay,
             reward_unseen_speed,
             reward_seen_speed,
             reward_unseen_speed * uncertainty,
             reportfile,resultsfile)

print("4 - DRN is suppressed, less delay")
doExperiment(shorter_food_delay,
             reward_unseen_speed,
             reward_seen_speed,
             reward_unseen_speed * uncertainty,
             reportfile,resultsfile)

print("5 - DRN is suppressed and SSRI")
reward_unseen_speed = 100 * ofc5HTreceptors(0.3, vis1, 2)
reward_seen_speed = 100 * ofc5HTreceptors(0.3, vis1+vis2, 2)
doExperiment(standard_food_delay,
             reward_unseen_speed,
             reward_seen_speed,
             reward_unseen_speed * uncertainty,
             reportfile,resultsfile)

print("6 - DRN is suppressed and SSRI, less delay")
doExperiment(shorter_food_delay,
             reward_unseen_speed,
             reward_seen_speed,
             reward_unseen_speed * uncertainty,
             reportfile,resultsfile)

print("7 - DRN is suppressed and 5HTR2 up")
reward_unseen_speed = 100 * ofc5HTreceptors(0.3, vis1, 3)
reward_seen_speed = 100 * ofc5HTreceptors(0.3, vis1+vis2, 3)
doExperiment(standard_food_delay,
             reward_unseen_speed,
             reward_seen_speed,
             reward_unseen_speed * uncertainty,
             reportfile,resultsfile)

print("8 - DRN is suppressed and 5HTR2 up, less delay")
doExperiment(shorter_food_delay,
             reward_unseen_speed,
             reward_seen_speed,
             reward_unseen_speed * uncertainty,
             reportfile,resultsfile)

reportfile.close()
resultsfile.close()
