source ~/python_projects/.venv/bin/activate
python3 plot.py analog1
python3 plot.py "analog1/4.1.4_-3" --ch1 "V_{i+}" --ch2 "V_o" --const-label "V_{i-}" --const-value -3
python3 plot.py "analog1/4.1.4_2/" --ch1 "V_{i+}" --ch2 "V_o" --const-label "V_{i-}" --const-value 2
python3 plot.py "analog1/4.1.4_0" --ch1 "V_{i+}" --ch2 "V_o" --const-label "V_{i-}" --const-value 0
python3 plot.py "analog1/4.5.4" --ch1 "V_{i1}" --ch2 "V_o" --const-label "V_{i2}" --const-value -1
 python3 plot.py "analog1/4.5.5" --ch1 V_{i1} V_{i2} --ch2 V_o
python3 plot.py "analog1/4.6.4" --ch1 "V_{i1}" --ch2 "V_o" --const-label "V_{i2}" --const-value -1
python3 plot.py "analog1/4.6.5" --ch1 "V_{i1}" --ch2 "V_o" --const-label "V_{i2}" --const-value 1
python3 collect_combined.py ./analog1
