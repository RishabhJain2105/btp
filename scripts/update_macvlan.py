import yaml
import sys
from pathlib import Path
import os


def create_network_attachment(namespace, with_ip=False):
    base_config = {
        "apiVersion": "k8s.cni.cncf.io/v1",
        "kind": "NetworkAttachmentDefinition",
        "metadata": {
            "name": "core5g-def",
            "namespace": namespace
        },
        "spec": {
            "config": {
                "cniVersion": "0.3.1",
                "plugins": [
                    {
                        "type": "macvlan",
                        "capabilities": {"ips": True},
                        "master": "eth1",
                        "mode": "bridge",
                        "ipam": {
                            "type": "static"
                        }
                    },
                    {"type": "tuning"}
                ]
            }
        }
    }

    if with_ip:
        base_config["spec"]["config"]["plugins"][0]["ipam"] = {
            "type": "static",
            "addresses": [
                {
                    "address": "10.0.3.1/24",
                    "gateway": "10.0.3.254"
                }
            ]
        }

    # Convert `config` dict to JSON-like string, as in your original YAML
    base_config["spec"]["config"] = yaml.dump(base_config["spec"]["config"], sort_keys=False)
    return base_config


def main():
    if len(sys.argv) != 2:
        print("Usage: python generate_macvlan_yaml.py <n>")
        sys.exit(1)

    n = int(sys.argv[1])
    output_file = Path(os.path.dirname(__file__)).resolve().parent / "5gcore-sctp-loadbalancer" / "core-5g-macvlan.yml"

    docs = []
    # 1️⃣ core5g-def in open5gs
    docs.append(create_network_attachment("open5gs"))

    # 2️⃣ n ran-simulator namespaces
    for i in range(1, n + 1):
        docs.append(create_network_attachment(f"ran-simulator{i}"))

    # 3️⃣ loadbalancer with IP address
    docs.append(create_network_attachment("loadbalancer", with_ip=True))

    # Write all YAML docs separated by ---
    with open(output_file, "w") as f:
        yaml.dump_all(docs, f, sort_keys=False)

    print(f"✅ Generated {output_file} with {n + 2} YAML documents.")


if __name__ == "__main__":
    print("This is not to be used...")
    print("Run 5gcore-sctp-loadbalancer/macvlan instead.")
    # main()
