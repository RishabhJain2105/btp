import sys
import yaml

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <num_ues>")
        sys.exit(1)

    n = int(sys.argv[1])

    # Base structure
    configmap = {
        "apiVersion": "v1",
        "kind": "ConfigMap",
        "metadata": {
            "name": "{{ .Release.Name }}-mongo-ue-init",
            "labels": {
                "epc-mode": "job"
            }
        },
        "data": {
            "ue-init.sh": (
                "wget https://github.com/open5gs/open5gs/raw/v2.5.6/misc/db/open5gs-dbctl\n"
                "chmod +x open5gs-dbctl\n\n"
            )
        }
    }

    # Add UE sections
    script_lines = [configmap["data"]["ue-init.sh"]]

    for i in range(1, n + 1):
        script_lines.append(
f"""if ./open5gs-dbctl --db_uri=mongodb://{{{{ .Release.Name }}}}-mongodb-svc/open5gs showfiltered | grep -w {{{{ .Values.simulator.ue{i}.imsi }}}}; then
     echo "UE {{{{ .Values.simulator.ue{i}.imsi }}}} exists, proceeding to delete"
     ./open5gs-dbctl --db_uri=mongodb://{{{{ .Release.Name }}}}-mongodb-svc/open5gs remove {{{{ .Values.simulator.ue{i}.imsi }}}}
     ./open5gs-dbctl --db_uri=mongodb://{{{{ .Release.Name }}}}-mongodb-svc/open5gs add_ue_with_slice {{{{ .Values.simulator.ue{i}.imsi }}}} {{{{ .Values.simulator.ue{i}.secKey }}}} {{{{ .Values.simulator.ue{i}.op }}}} {{{{ .Values.dnn }}}} {{{{ .Values.simulator.ue{i}.sst }}}} {{{{ .Values.simulator.ue{i}.sd }}}};
else
     echo "UE {{{{ .Values.simulator.ue{i}.imsi }}}} does not exist in the DB, proceeding to add it"
     ./open5gs-dbctl --db_uri=mongodb://{{{{ .Release.Name }}}}-mongodb-svc/open5gs add_ue_with_slice {{{{ .Values.simulator.ue{i}.imsi }}}} {{{{ .Values.simulator.ue{i}.secKey }}}} {{{{ .Values.simulator.ue{i}.op }}}} {{{{ .Values.dnn }}}} {{{{ .Values.simulator.ue{i}.sst }}}} {{{{ .Values.simulator.ue{i}.sd }}}};
fi

"""
        )

    # Join script into one string
    configmap["data"]["ue-init.sh"] = "".join(script_lines)

    # Dump YAML
    file_path = "mongo-ue-init-script.yaml"
    with open(file_path, "w") as f:
        yaml.dump(configmap, f, sort_keys=False)

    print(f"{file_path} generated successfully.")

if __name__ == "__main__":
    main()
