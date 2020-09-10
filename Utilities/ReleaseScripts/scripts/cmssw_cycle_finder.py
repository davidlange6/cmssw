#!/usr/bin/env python

# find cycles in cmssw libs
import collections

class Graph(object):
    def __init__(self, edges):
        self.edges = edges

    @staticmethod
    def _build_adjacency_list(edges):
        adj = collections.defaultdict(list)
        for edge in edges:
            adj[edge[0]].append(edge[1])
        return adj

    def addEdge(self,edge):
        self.edges.append(edge)

    def build_adjacency_list(self):
        self.adj = Graph._build_adjacency_list(self.edges)
        

def dfs(G):
    #print(G.edges)
    discovered = set()
    finished = set()
    for u in G.adj:
        if u not in discovered and u not in finished:
            discovered, finished = dfs_visit(G, u, discovered, finished)

def dfs_visit(G, u, discovered, finished):
    if u not in G.adj:
        finished.add(u)
        return discovered, finished

    discovered.add(u)

    for v in G.adj[u]:
        # Detect cycles
        if v in discovered:
            print(f"Cycle detected: found a back edge from {u} to {v}.")

        # Recurse into DFS tree
        else:
            if v not in finished:
                dfs_visit(G, v, discovered, finished)

    discovered.remove(u)
    finished.add(u)

    return discovered, finished

import subprocess
def run_command(comm):
    encoding = 'utf-8'
    proc = subprocess.Popen(comm, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True)
    stdout, stderr = proc.communicate()
    stdout = stdout.decode(encoding)
    if stderr is not None:
        stderr= stderr.decode(encoding)
    return proc.returncode, stdout, stderr


import os
def get_lib_list():
    if 'CMSSW_RELEASE_BASE' not in os.environ:
        print("Execute within a cmssw environment")
        return []

    lib_base = os.environ.get('CMSSW_RELEASE_BASE')
    arch = os.environ.get('SCRAM_ARCH')
    lib_path = os.path.join(lib_base,"lib",arch)
    found_libs={}
    comm = "ls "+lib_path+"| grep so | grep lib"
    c_rt,c_out,c_err = run_command(comm)

    ret_val=[]
    for l in c_out.split():
#        print(l)
        if l.startswith("lib"):
            ret_val.append( (l,os.path.join(lib_path,l)))
            found_libs[l]=1

    if 'CMSSW_FULL_RELEASE_BASE' in os.environ:
        lib_base = os.environ.get('CMSSW_FULL_RELEASE_BASE')
        lib_path = os.path.join(lib_base,"lib",arch)
        comm = "ls "+lib_path+"| grep so | grep lib"
        c_rt,c_out,c_err = run_command(comm)
        for l in c_out.split():
            if l.startswith("lib") and l not in found_libs:
                ret_val.append( (l,os.path.join(lib_path,l)))
                found_libs[l]=1

    return ret_val

def get_lib_deps(lib_info):
    lib_path = lib_info[1]
    comm = "ldd "+lib_path+"| grep cms | grep -v \"/external\" | grep -v \"/lcg/\" | awk '{print $3}'"
    c_rt,c_out,c_err = run_command(comm)
    ret_val=[]
    for l in c_out.split():
        lib = l.strip()
        ret_val.append( (lib.split('/')[-1],lib))
    return ret_val


import sys

if __name__ == "__main__":
    G = Graph([])

    lib_list = get_lib_list()
    for lib in lib_list:
        print(lib[0])
        deps = get_lib_deps(lib)
        for dep in deps:
            G.addEdge( (lib[0],dep[0]) )

    print("Building adj")
    G.build_adjacency_list()
    print("Looking for cycles")
    dfs(G)

