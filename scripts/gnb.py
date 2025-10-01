import os
import sys
import yaml

def create_file(file_path, content):
    """Create a file with the given content."""
    with open(file_path, "w") as f:
        f.write(content)

def create_folder(folder_num, base_ip):
    folder_name = f"my5GRanTester{folder_num}-helm-chart"
    os.makedirs(folder_name, exist_ok=True)

    # Chart.yaml content
    chart_yaml_content = {
        'apiVersion': 'v2',
        'name': 'my5GRanTester',
        'description': 'A Helm chart for my5GRanTester',
        'type': 'application',
        'version': '0.0.1',
        'appVersion': '0.0.1'
    }
    create_file(os.path.join(folder_name, 'Chart.yaml'), yaml.dump(chart_yaml_content))

    # Calculate controlDataifIP
    control_data_ip = f"10.0.3.{10 + folder_num - 1}"

    # values.yaml content
    values_yaml_content = {
        'image': {
            'repository': 'wriddhiraaj/my5g-ran-tester',
            'pullPolicy': 'IfNotPresent',
            'tag': 'latest'
        },
        'config': {
            'amfVIP': '10.0.3.1',
            'amfPort': '38412',
            'controlDataifIP': control_data_ip,
            'controlDataifNetMask': '24',
            'msin': '0000000031',
            'mcc': '208',
            'mnc': '93',
            'tac': '000007',
            'opc': '63bfa50ee6523365ff14c1f45f88737d',
            'key': '0C0A34601D4F07677303652C0462535B',
            'dnn': 'internet',
            'sst': '01',
            'sd': '000001'
        }
    }
    create_file(os.path.join(folder_name, 'values.yaml'), yaml.dump(values_yaml_content))

    # templates folder
    templates_folder = os.path.join(folder_name, 'templates')
    os.makedirs(templates_folder, exist_ok=True)

    # rantester-configmap.yaml content (Helm templated)
    rantester_configmap_content = """apiVersion: v1
kind: ConfigMap
metadata:
  name: {{ .Release.Name }}-config
  labels:
    app: rantester
data:
  config.yml: |
    gnodeb:
      controlif:
        ip: "{{ .Values.config.controlDataifIP }}"
        port: 9487
      dataif:
        ip: "{{ .Values.config.controlDataifIP }}"
        port: 2152
      plmnlist:
        mcc: "{{ .Values.config.mcc }}"
        mnc: "{{ .Values.config.mnc }}"
        tac: "{{ .Values.config.tac }}"
        gnbid: "0000001"
      slicesupportlist:
        sst: "{{ .Values.config.sst }}"
        sd: "{{ .Values.config.sd }}"

    ue:
      msin: "{{ .Values.config.msin }}"
      key: "{{ .Values.config.key }}"
      opc: "{{ .Values.config.opc }}"
      amf: "8000"
      sqn: "0000000"
      dnn: "{{ .Values.config.dnn }}"
      hplmn:
        mcc: "{{ .Values.config.mcc }}"
        mnc: "{{ .Values.config.mnc }}"
      snssai:
        sst: {{ .Values.config.sst }}
        sd: "{{ .Values.config.sd }}"
      integrity:
        nia0: false
        nia1: false
        nia2: true
      ciphering:
        nea0: true
        nea1: false
        nea2: false
    amfif:
      ip: "{{ .Values.config.amfVIP }}"
      port: {{ .Values.config.amfPort }}

    logs:
      level: 4
"""
    create_file(os.path.join(templates_folder, 'rantester-configmap.yaml'), rantester_configmap_content)

    # rantester-deployment.yaml content
    rantester_deployment_content = """apiVersion: apps/v1
kind: Deployment
metadata:
  name: {{ .Release.Name }}-simulator
  labels:
    app: ran-tester
spec:
  replicas: 1
  selector:
    matchLabels:
      app: ran-tester
  template:
    metadata:
      annotations:
         k8s.v1.cni.cncf.io/networks: '[
                 { "name": "core5g-def",
                   "ips": [ {{- cat .Values.config.controlDataifIP "/" .Values.config.controlDataifNetMask | nospace | quote }} ] }
         ]'
      labels:
        mode: simulator
        app: ran-tester
    spec:
      containers:
      - name: ran
        image: "{{ .Values.image.repository }}:{{ .Values.image.tag }}"
        imagePullPolicy: {{ .Values.image.pullPolicy }}
        command: ["/bin/sh", "-c"]
        args:
        - ip route add {{ .Values.config.amfVIP }};
          sleep infinity;
        securityContext:
          privileged: true
        volumeMounts:
        - name: ran-tester-config
          mountPath: /root/go/src/my5G-RANTester/config/config.yml
          subPath: config.yml
      volumes:
        - name: ran-tester-config
          configMap:
            name: {{ .Release.Name }}-config
"""
    create_file(os.path.join(templates_folder, 'rantester-deployment.yaml'), rantester_deployment_content)

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <number_of_UEs>")
        sys.exit(1)

    num_folders = int(sys.argv[1])
    for i in range(1, num_folders + 1):
        create_folder(i, "10.0.3.10")

    print("Folders created successfully.")

if __name__ == "__main__":
    main()
