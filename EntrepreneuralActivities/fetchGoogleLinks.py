from bs4 import BeautifulSoup
import urllib
import requests
#r = urllib.urlopen('https://www.google.com/?gws_rd=ssl#q=cpa+firms+in+california').read()
#soup = BeautifulSoup(r)
#print type(soup)
#print soup.prettify()[0:1000]
#print soup.prettify()[0:100]
#print soup.prettify()[0:100000]
#from IPython.display import import Image                                                                                                             [36/1731]
#Image('http://www.openbookproject.net/tutorials/getdown/css/images/lesson4/HTMLDOMTree.png')
#links = soup.find_all("a", id="vs0p*")
#links[0]
#links[1]
#print type(links)
#links = soup.find_all("li", class="ads-ad")
#links = soup.find_all("li", class_="ads-ad")
#links[0].
#links[0]
#links[1]
#I got a different URL than Rob M. when I tried searching with JavaScript disabled -
#https://www.google.com/search?q=site:wikipedia.com+Thomas+Jefferson&gbv=1&sei=YwHNVpHLOYiWmQHk3K24Cw
#To make this work with any query, you should first make sure that your query has no spaces in it (that's why you'll get a 400: Bad Request). You can do this with a replace like this:
#query = "Thomas Jefferson"
#query = query.replace(' ', '+')
# which will replace all of the spaces with plus signs - creating a valid URL. Note here that you need to reassign the output of the replace method to qu ery - str.replace() does not modify the string in place, it returns a copy of the string with the modifications made.  However, this does not work with urllib - you get a 403: Forbidden. I got it to work by using the python-requests module like this:
#query = 'Thomas Jefferson'
#query = query.replace (" ", "+")
##replaces whitespace with a plus sign for Google compatibility purposes
#r = requests.get('https://www.google.com/search?q=site:wikipedia.com+{}&gbv=1&sei=YwHNVpHLOYiWmQHk3K24Cw'.format(query))
def getLinksFromGoogle():
    r = requests.get('https://www.google.com/search?q=cpa+firms+in+california')
    soup = BeautifulSoup(r.text, "html.parser")
#creates soup and opens URL for Google. Begins search with site:wikipedia.com so only wikipedia
#links show up. Uses html parser.
    links = []
    for item in soup.find_all('h3', attrs={'class' : 'r'}):
         links.append(item.a['href'][7:]) # [7:] strips the /url?q= prefix
             #Guides BS to h3 class "r" where green Wikipedia URLs are located, then prints URLs
                 #Limiter code to only pull top 5 results
#print links
#soup = BeautifulSoup(r.text, "html.parser")
#links = []
#for item in soup.find_all('h3', attrs={'class' : 'r'}):
#    links.append(item.a['href'][7:]) # [7:] strips the /url?q= prefix
#links.append(item.a['href'][7:]) # [7:] strips the /url?q= prefix
#Guides BS to h3 class "r" where green Wikipedia URLs are located, then prints URLs
#Limiter code to only pull top 5 results
    print links[0].split('&')[0]
    decodedLinks = []
    for link in links:
        decodedLinks.append(urllib.unquote(link.split('&')[0]))

    return decodedLinks

def main():
    links = getLinksFromGoogle()
    for link in links:
        print link

if __name__ == "__main__":
    main()
