### Crawler

####Description
Given a link, crawler will recursively parse webpages for URLs and download their HTMLS.  All files are downloaded to target directory and the depth of the search is specified by the user.

####To run crawler
Usage: <br>
``./crawler [SEED URL] [TARGET DIRECTORY TO DEPOSIT DATA] [CRAWLING DEPTH]``<br>
Example:<br>
``./crawler http://old-www.cs.dartmouth.edu/~cs50/tse ./crawledFiles 3``<br>
Impt Note: crawler is currently limited to crawling pages within the above domain, but this limitation can be removed by commenting out lines 78-81 in crawler.c

For further information, refer to crawler's [documentation](./Crawler Documentation)
