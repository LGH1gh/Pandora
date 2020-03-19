import sys, os

class Vertex:

  def __init__(self, position):
    self.position = position

  def __str__(self):
    return ('{ ' + ', '.join(self.position) + ' }, ')
  
  def toString(self):
    return ('{ ' + ', '.join(self.position) + ' }, ')
    
class Index:
  def __init__(self, vertexIndex):
    self.vertexIndex = vertexIndex
  
  def __str__(self):
    vertexIndexStr = list(map(str, self.vertexIndex))
    return (', '.join(vertexIndexStr) + ',')

  def toString(self):
    vertexIndexStr = list(map(str, self.vertexIndex))
    return (', '.join(vertexIndexStr) + ',')

vertices = []
indices = []

objFile = open('./utah-teapot.obj', mode='r', encoding='utf-8')

for line in objFile.readlines():
  metaData = line.strip('\n').split(' ')
  if metaData[0] == 'v':
    vertices.append(Vertex(metaData[1:]))
  if metaData[0] == 'f':
    vertexIndex = []
    for data in metaData[1:]:
      vertexIndex.append(data.split('/')[0])
    if len(vertexIndex) == 4:
      indices.append(Index([(int)(vertexIndex[0]) - 1, (int)(vertexIndex[1]) - 1, (int)(vertexIndex[2]) - 1]))
      indices.append(Index([(int)(vertexIndex[0]) - 1, (int)(vertexIndex[2]) - 1, (int)(vertexIndex[3]) - 1]))
    else :
      indices.append(Index(vertexIndex))

objFile.close()

vertexFile = open('./vertex.txt', mode='w', encoding='utf-8')
for vertex in vertices:
  vertexFile.write(vertex.toString() + '\n')
vertexFile.close()

indexFile = open('./index.txt', mode='w', encoding='utf-8')
for index in indices:
  indexFile.write(index.toString() + '\n')
indexFile.close()
