import os
import yaml
import sys


def generate_yaml(n, filename):
    # Base IMSI for 15-digit IDs
    base_imsi = 208930000000000

    simulator = {}
    for i in range(1, n + 1):
        simulator[f'ue{i}'] = {
            'imsi': str(base_imsi + i),
            'imei': '356938035643803',
            'imeiSv': '4370816125816151',
            'op': '63bfa50ee6523365ff14c1f45f88737d',
            'secKey': '0C0A34601D4F07677303652C0462535B',
            'sst': '1',
            'sd': '1'
        }

    data = {
        'open5gs': {
            'image': {
                'repository': 'registry.gitlab.com/infinitydon/registry/open5gs-aio',
                'pullPolicy': 'IfNotPresent',
                'tag': 'v2.5.6'
            }
        },
        'webui': {
            'image': {
                'repository': 'registry.gitlab.com/infinitydon/registry/open5gs-webui',
                'pullPolicy': 'IfNotPresent',
                'tag': 'v2.5.6'
            }
        },
        'ueImport': {
            'image': {
                'repository': 'free5gmano/nextepc-mongodb',
                'pullPolicy': 'IfNotPresent',
                'tag': 'latest'
            }
        },
        'simulator': simulator,
        'dnn': 'internet',
        'k8swait': {
            'repository': 'groundnuty/k8s-wait-for',
            'tag': 'v1.6',
            'pullPolicy': 'IfNotPresent'
        },
        'k8s': {'interface': 'eth0'},
        'amf1': {'mcc': 208, 'mnc': 93, 'tac': 7, 'networkName': 'Open5GS', 'ngapInt': 'net1', 'multusN2IP': '10.0.3.3', 'multusN2NetworkMask': 24},
        'amf2': {'mcc': 208, 'mnc': 93, 'tac': 7, 'networkName': 'Open5GS', 'ngapInt': 'net1', 'multusN2IP': '10.0.3.4', 'multusN2NetworkMask': 24},
        'amf3': {'mcc': 208, 'mnc': 93, 'tac': 7, 'networkName': 'Open5GS', 'ngapInt': 'net1', 'multusN2IP': '10.0.3.5', 'multusN2NetworkMask': 24},
        'smf': {'N4Int': 'net1', 'multusN4IP': '10.0.3.6', 'multusN4NetworkMask': 24},
        'upf': {'N3N4Int': 'net1', 'multusN3N4IP': '10.0.3.7', 'multusN3N4NetworkMask': 24, 'multusN3N4GW': '10.0.3.1'},
        'nssf': {'sst': '1', 'sd': '1'},
        'prometheus': {'nodeExporter': {'repository': 'quay.io/prometheus/node-exporter', 'tag': 'v1.3.1', 'pullPolicy': 'IfNotPresent'}}
    }

    with open(filename, 'w') as f:
        yaml.dump(data, f, default_flow_style=False, sort_keys=False)


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 addue.py <number_of_ues>")
        sys.exit(1)
    n = int(sys.argv[1])
    filename = os.path.abspath(
        os.path.join(
            "5gcore-sctp-loadbalancer",
            "open5gs-helm-charts",
            "values.yaml"))
    generate_yaml(n, filename)
