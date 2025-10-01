read -p "How many UEs do you want to simulate? " num_ue

python scripts/addue.py $num_ue
python scripts/time_setter.py $num_ue
python scripts/mongo.py $num_ue
python scripts/gnb.py $num_ue