import sys, os

class Vertex:

  def __init__(self, position, normal):
    self.position = position
    self.normal = normal

  def __str__(self):
    return ('{ { ' + ', '.join(self.position) + ' }, { ' + ', '.join(self.normal) + '} }, ')
  
  def toString(self):
    return ('{ { ' + ', '.join(self.position) + ' }, { ' + ', '.join(self.normal) + '} }, ')
    
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

position = []
normal = []

objFile = open('./utah-teapot.obj', mode='r', encoding='utf-8')

for line in objFile.readlines():
  metaData = line.strip('\n').split(' ')
  if metaData[0] == 'v':
    position.append(metaData[1:])
  if metaData[0] == 'vn':
    normal.append(metaData[1:])
  if metaData[0] == 'f':
    positionIndex = 0
    normalIndex = 0
    for data in metaData[1:]:
      positionIndex = (int)(data.split('/')[0]) - 1
      normalIndex = (int)(data.split('/')[2]) - 1
      vertices.append(Vertex(position[positionIndex], normal[normalIndex]))
    indices.append(Index([len(vertices) - 4, len(vertices) - 3, len(vertices) - 2]))
    indices.append(Index([len(vertices) - 4, len(vertices) - 2, len(vertices) - 1]))

objFile.close()

vertexFile = open('./vertex.txt', mode='w', encoding='utf-8')
vertexFile.write('vertex = {\n')
for vertex in vertices:
  vertexFile.write('    ' + vertex.toString() + '\n')
vertexFile.write('}\n')
vertexFile.close()

indexFile = open('./index.txt', mode='w', encoding='utf-8')
indexFile.write('index = {\n')
for index in indices:
  indexFile.write('    ' + index.toString() + '\n')
indexFile.write('}\n')
indexFile.close()
