import subprocess
import re
import matplotlib.pyplot as plt
import sys
import numpy
from scipy.stats import bootstrap

def rodar(arq,rep,Tamanho,programa):
	aux=1
	while(aux<=rep):			#Quantidade de repeticoes
		print("Repetição "+str(aux)+" de "+str(rep)) 
		for size0 in Tamanho:		#Variados tamanhos
				teste=subprocess.run([programa, str(size0)], stdout=subprocess.PIPE)
				linhas = teste.stdout.splitlines()
				salvarRAW(arq,size0,aux,linhas)
		aux+=1
		
	return

def GrafSpeedup(arq,x,y,error1):
	Sizex=[]
	for valor in x:
		Sizex.append(str(valor))
	plt.plot(x, y, color = 'navy')
	plt.xticks(x,Sizex)
	plt.savefig(arq+"/grafico.pdf")
	
	


def procura(txt,arq):
	for line in arq:
		line=str(line)
		if(txt in line):		#Procura a variavel
			line=enumera(line)
			return int(line)
			

def graf_comparacao(arq,x,y,y1,y2,error1,error2,error3):
	print("High\t Low")
	print(error3)
	print("primeiro vetor")
	print(numpy.rot90(error3)[0])
	print("segundo vetor")
	print(numpy.rot90(error3)[1])
	Sizex=['128', '256', '512', '1024', '2048','4096','8192','16384','32768','65536']
	plt.rcParams['figure.figsize'] = (11,7)
	plt.bar(x, y, yerr=numpy.rot90(error1), color='r')
	plt.bar(x, y1, yerr=numpy.rot90(error2), color='b')
	plt.bar(x, y2, yerr=numpy.rot90(error3), color='g')
	plt.ylabel('Trocas')
	plt.xlabel('Tamanho (Número de elementos no conjunto)')
	#plt.title('Comparação PM vs RAM')
	#plt.legend(labels=['DRAM', 'PM','SSD'],loc='upper center', bbox_to_anchor=(0.45, -0.1),fancybox=True, shadow=False, ncol=5)
	plt.legend(labels=['Insertion Sort', 'Selection Sort','...'])
	plt.savefig(arq+"/graficoComparacao.pdf",bbox_inches='tight',pad_inches = 0.02)
	#plt.show()

	
def salvarRAW(diretorio,size,num,text):
	arq=open(diretorio+"/teste"+str(size)+"_"+str(num),"w")	#Nome do arquivo
	for line in text:
		arq.write(str(line)+"\n")				#Salva as linhas
	return

def abrirRAW(txt,diretorio,rep,indice):
	listaAux=[]
	lista=[]
	listaTam=[]
	contador=0
	contador2=1
	contadorTam=0
	tamanho=indice[0]
	while(contador<(len(indice))):
		contador2=1
		tamanho=indice[contadorTam]
		listaAux=[]
		while(contador2<=rep):
			arq=open(diretorio+"/teste"+str(tamanho)+"_"+str(contador2),"r")
			aux=arq.read()
			aux=aux.splitlines()
			listaAux.append(procura(txt,aux))
			contador2+=1
		listaTam.append(tamanho)
		lista.append(listaAux)		
		contadorTam+=1
		contador+=1
	return listaTam,lista
			
	
def mkdir(arq):				#Cria o diretorio
	aux=1
	teste=subprocess.run(["mkdir",arq],stdout=subprocess.PIPE)
	return arq
	'''if(teste.returncode==0):
		return arq
	else:
		while(teste.returncode!=0):
			arq0=arq+str(aux)
			teste=subprocess.run(["mkdir",arq0],stdout=subprocess.PIPE)
			aux+=1
		return arq0	'''

def criaIndice(arq,rep,Tamanho):
	arquivo=open(arq+"/indice.txt","w")			#Salva os tamanhos e quantidade de testes
	arquivo.write("["+str(rep)+","+str(Tamanho)+']')
	return

def abrirIndice(arq):
	arquivo=open(arq+"/indice.txt","r")		#Resgata os tamanhos e quantidade de testes
	lista=arquivo.read()
	lista=eval(lista)
	return lista

def desvioPadrao(lista):		#Calcula desvio Padrao
	x=(sum(lista))/len(lista)
	contador=0
	total=0
	while(contador<len(lista)):
		total=((lista[contador]-x)**2)+total
		contador+=1
	total=total/len(lista)
	total=total**(1/2)
	return total

def IntConfianca(lista):
	Z=1.96				#Representa o valor da distribuição normal padrão para o nível de confiança de 95%
	desvio=desvioPadrao(lista)
	return(Z*(desvio/(len(lista)**(1/2))))
	
def mediaLista(listas):
	medias=[]
	for lista in listas:
		medias.append((sum(lista))/len(lista))
	return medias

def enumera(lista):					#Remove os textos,deixando apenas os numeros
	retornar=[]
	if(type(lista) is list):
		for line in lista:
			valor=''
			line=str(line)
			line=re.sub(r'Trocas: ','',line)
			retornar.append(eval(line))
	else:
		valor=''
		lista=str(lista)
		lista=re.sub(r'Trocas: ','',lista)
		retornar=eval(lista)
		
	return retornar

def main(arg):
	rep=10					#Quantas vezes ira repetir
	'''
	tam_ini=128
	aumentar=15
	tamanho=[]
	while(aumentar>0):
		tamanho.append(tam_ini)
		tam_ini=tam_ini*2
		aumentar-=1
	'''
	tamanho=[128,256,512,1024,2048,4096,8192,16384,32768,65536]
	print(tamanho)
	programa="./trab-selection"
	arq="test_selection"
	txt='Trocas: '
	arq=mkdir(arq)
	x=[]
	intervalo=[]
	if(len(arg)==1 or len(arg)>7):
		print("Modo de uso: python3 bash.py <argumento>\nDigite <python3 bash.py h> para lista de comandos")
		return
	
	elif(arg[1]=='h'):
		print("Comandos:\ng --> Plota o grafico com os dados prontos\na --> Roda o programa e produz o grafico em seguida\nr --> Apenas roda o programa")
		return	
	
	elif(arg[1]=='r'):
		criaIndice(arq,rep,tamanho)
		rodar(arq,rep,tamanho,programa)
		return		

	elif(arg[1]=='g'):
		intervalo=[]
		indice=abrirIndice(arq)
		rep=indice[0]
		indice.pop(0)
		indice=indice[0]
		x,y=abrirRAW(txt,arq,rep,indice)
		for lista in y:
			data = (lista,)
			#intervalo.append(IntConfianca(lista))
			var=(bootstrap(data, numpy.std, confidence_level=0.95))
			intervalo.append([var.confidence_interval[1],var.confidence_interval[0]])
		GrafSpeedup(arq,x,y,numpy.array(intervalo))

	elif(arg[1]=='c'):
		intervalo=[]
		intervalo2=[]
		intervalo3=[]
		arq2=' '
		arq3=' '
		indice=abrirIndice(arq)
		indice2=abrirIndice(arq2)
		indice3=abrirIndice(arq3)
		rep=indice[0]
		indice.pop(0)
		indice=indice[0]
		rep2=indice2[0]
		indice2.pop(0)
		indice2=indice2[0]
		rep3=indice3[0]
		indice3.pop(0)
		indice3=indice3[0]
		x,y=abrirRAW(txt,arq,rep,indice)
		x2,y2=abrirRAW(txt,arq2,rep2,indice2)
		x3,y3=abrirRAW(txt,arq3,rep3,indice3)
		medias=mediaLista(y)
		medias2=mediaLista(y2)
		medias3=mediaLista(y3)
		#GrafSpeedup(arq,x,medias,medias2,medias3)#y1=Ram,y2=PM,y3=ssd
		for lista in y:
			data = (lista,)
			#intervalo.append(IntConfianca(lista))
			var=(bootstrap(data, numpy.std, confidence_level=0.95))
			intervalo.append([var.confidence_interval[1],var.confidence_interval[0]])
		for lista in y2:
			#intervalo2.append(IntConfianca(lista))
			var=(bootstrap(data, numpy.std, confidence_level=0.95))
			intervalo2.append([var.confidence_interval[1],var.confidence_interval[0]])
		for lista in y3:
			data = (lista,)
			#intervalo3.append(IntConfianca(lista))
			var=(bootstrap(data, numpy.std, confidence_level=0.95))
			intervalo3.append([var.confidence_interval[1],var.confidence_interval[0]])
		graf_comparacao(arq,x,medias,medias2,medias3,numpy.array(intervalo),numpy.array(intervalo2),numpy.array(intervalo3))
		return
		
	else:
		print("Comando errado\nDigite <python3 bash.py h> para lista de comandos")
		return
	
	
main(sys.argv)
