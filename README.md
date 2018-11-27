# kube-netcheck-c

Trivial network checker for Kubernetes written in C.

Initially was written in Go [here](https://github.com/giantswarm/kube-netcheck).

# Quick start

```
kubectl apply -f https://raw.githubusercontent.com/r7vme/kube-netcheck-c/master/examples/quickstart.yaml
```
```
sleep 60; kubectl get pods -l app=kube-netcheck -n kube-system
```
