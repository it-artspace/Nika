
#ifndef EmAlgorithm_h
#define EmAlgorithm_h
#include "algorithmsControl.h"
#include <map>
#include <cstdio>
#include "EMConsumer.h"

#include<vector>

#include <math.h>

typedef struct inputdata_multi_gauss {
    double * pData;
    int nClass;
    double * pNormalProb;
} INPUTDATA_MULTI_GAUSS;
//klass dlya obrabotki dannyh s sostoyaniem
class CGMM
{
    EMConsumer writer;
    //dolzhny ostavatsa takimi zhe dlya korrektnoy otrisovki
    std::vector<int> clust_colors;
public:
    CGMM(void){
        m_nMaxLoop = 10000;
        
        m_pNumClass = 0;
        m_pProbClass = 0;
        
        m_ppSumFeatClass = 0;
        m_ppSumVarClass = 0;
        m_ppMeanFeatClass = 0;
        m_ppVarFeatClass = 0;
    }
    ~CGMM(void){
        delete[] m_pNumClass;
        delete[] m_pProbClass;
        
        for(int a=0; a<m_nSizeFeature; a++) {
            delete[] m_ppSumFeatClass[a];
            delete[] m_ppSumVarClass[a];
            delete[] m_ppMeanFeatClass[a];
            delete[] m_ppVarFeatClass[a];
        }
        delete[] m_ppSumFeatClass;
        delete[] m_ppSumVarClass;
        delete[] m_ppMeanFeatClass;
        delete[] m_ppVarFeatClass;
    }
    
    void init(int nSizeK, int nSizeRecord, int nSizeFeature, INPUTDATA_MULTI_GAUSS ** ppDataList){
        m_nSizeK = nSizeK;
        
        m_nSizeRecord = nSizeRecord;
        for(int i = 0; i < nSizeK; i++){
            clust_colors.push_back(rand() % (1<<24));
        }
        m_nSizeFeature = nSizeFeature;
        m_ppDataList = ppDataList;
        
        m_pNumClass = new int[m_nSizeK];
        m_pProbClass = new double[m_nSizeK];
        m_ppSumFeatClass = new double*[m_nSizeK];
        m_ppSumVarClass = new double*[m_nSizeK];
        m_ppMeanFeatClass = new double*[m_nSizeK];
        m_ppVarFeatClass = new double*[m_nSizeK];
        for(int a=0; a<m_nSizeK; a++) {
            m_pNumClass[a] = 0;
            m_pProbClass[a] = 0;
            
            m_ppSumFeatClass[a] = new double[m_nSizeFeature];
            m_ppSumVarClass[a] = new double[m_nSizeFeature];
            m_ppMeanFeatClass[a] = new double[m_nSizeFeature];
            m_ppVarFeatClass[a] = new double[m_nSizeFeature];
            for(int b=0; b<m_nSizeFeature; b++) {
                m_ppSumFeatClass[a][b] = 0;
                m_ppSumVarClass[a][b] = 0;
                m_ppMeanFeatClass[a][b] = 0;
                m_ppVarFeatClass[a][b] = 0;
            }
        }
    }
    void train(){
        i_step();
        
        for(int z=0; z<m_nMaxLoop; z++) {
            m_step();
            if(!m_bIsChangeClass)
                break;
            e_step();
        }
        printparameter();
    }
    
private:
    void i_step(){
        for(int a=0; a<m_nSizeRecord; a++) {
            // assign initial cluster(random)
            m_ppDataList[a]->nClass = a % m_nSizeK;
        }
        
        e_step();
    }
    void e_step(){
        // reset statistics
        for(int a=0; a<m_nSizeK; a++) {
            m_pNumClass[a] = 0;
            for(int b=0; b<m_nSizeFeature; b++) {
                m_ppSumFeatClass[a][b] = 0;
                m_ppSumVarClass[a][b] = 0;
            }
        }
        
        // count record & feature value
        for(int a=0; a<m_nSizeRecord; a++) {
            m_pNumClass[m_ppDataList[a]->nClass]++;
            for(int b=0; b<m_nSizeFeature; b++) {
                m_ppSumFeatClass[m_ppDataList[a]->nClass][b] += m_ppDataList[a]->pData[b];
            }
        }
        
        // calc mean
        for(int a=0; a<m_nSizeK; a++) {
            m_pProbClass[a] = (double)((double)m_pNumClass[a] / (double)m_nSizeRecord);
            for(int b=0; b<m_nSizeFeature; b++) {
                m_ppMeanFeatClass[a][b] = (double)((double)m_ppSumFeatClass[a][b] / (double)m_pNumClass[a]);
            }
        }
        
        // calc variance
        for(int a=0; a<m_nSizeRecord; a++) {
            for(int b=0; b<m_nSizeFeature; b++) {
                m_ppSumVarClass[m_ppDataList[a]->nClass][b] = m_ppSumVarClass[m_ppDataList[a]->nClass][b] +
                (m_ppDataList[a]->pData[b] - m_ppMeanFeatClass[m_ppDataList[a]->nClass][b]) *
                (m_ppDataList[a]->pData[b] - m_ppMeanFeatClass[m_ppDataList[a]->nClass][b]);
            }
        }
        
        for(int a=0; a<m_nSizeK; a++) {
            for(int b=0; b<m_nSizeFeature; b++) {
                m_ppVarFeatClass[a][b] = m_ppSumVarClass[a][b] / (double)(m_pNumClass[a] - 1);
            }
        }
        
        // print statistics
        std::vector<Cluster> clusters;
        for(int a=0; a<m_nSizeK; a++) {
            Cluster cl;
            cl.setColor(clust_colors[a]);
            for(int pointNo = 0; pointNo < m_nSizeRecord; pointNo++){
                if(m_ppDataList[pointNo]->nClass == a){
                    Point p(m_ppDataList[pointNo]->pData[0], m_ppDataList[pointNo]->pData[1]);
                    cl.addPoint(p);
                }
            }
            clusters.push_back(cl);
            
        }
        writer.write_chunk(clusters);
    }
    void m_step(){
        double * pProbability = new double[m_nSizeK];
        double dGauss = 1, dProbSum = 0;
        
        m_bIsChangeClass = false;
        
        for(int a=0; a<m_nSizeRecord; a++) {
            dProbSum = 0;
            
            for(int b=0; b<m_nSizeK; b++) {
                pProbability[b] = 1;
                dGauss = 1;
                
                for(int c=0; c<m_nSizeFeature; c++) {
                    dGauss = getgauss(m_ppMeanFeatClass[b][c], m_ppVarFeatClass[b][c], m_ppDataList[a]->pData[c]);
                    pProbability[b] *= dGauss;
                }
                dProbSum += pProbability[b];
            }
            
            double dTemp = 0;
            int nMaxId = 0;
            for(int b=0; b<m_nSizeK; b++) {
                m_ppDataList[a]->pNormalProb[b] = pProbability[b] / dProbSum;
                
                int nCurClass = m_ppDataList[a]->nClass;
                if(dTemp < pProbability[b]) {
                    nMaxId = b;
                    dTemp = pProbability[b];
                }
            }
            
            if(m_ppDataList[a]->nClass != nMaxId)
                m_bIsChangeClass = true;
            m_ppDataList[a]->nClass = nMaxId;
            
        }
        
        delete[] pProbability;
    }
    inline double getgauss(double dMean, double dVar, double dValue){
        double dGauss = 1;
        const double dPi = 3.14159265358979323846;
        
        dGauss = (1 / sqrt(2 * dPi * dVar)) * (exp((-1 * (dValue - dMean) * (dValue - dMean)) / (2*dVar)));
        
        return dGauss;
    }
    void printparameter(){
        for(int a=0; a<m_nSizeK; a++) {
            printf("P(c%d) = %0.3f \n", a, m_pProbClass[a]);
            for(int b=0; b<m_nSizeFeature; b++) {
                printf("Mean[%d][%d]=%.4f,\tVariance[%d][%d]=%.4f \n", a, b, m_ppMeanFeatClass[a][b], a, b, m_ppVarFeatClass[a][b]);
            }
        }
        printf("\n");
    }
    
    int m_nMaxLoop;
    
    // datas for training
    int m_nSizeK;                            // number of output pattern
    int m_nSizeFeature;                        // number of feature(column)
    int m_nSizeRecord;                        // number of records
    INPUTDATA_MULTI_GAUSS ** m_ppDataList;    // input datas of records
    
    bool m_bIsChangeClass;
    
    int * m_pNumClass;                        // number of records in each class
    double ** m_ppSumFeatClass;                // sum value of feature value in each class
    double ** m_ppSumVarClass;                // sum value of feature variation square in each class
    double ** m_ppMeanFeatClass;            // mean value of feature in each class
    double ** m_ppVarFeatClass;                // covariation value of feature in each class
    
    double * m_pProbClass;                    // 𝑷(𝒄)
};
                       
                       
                       
                                          











// update probability parameters


// assign clustering









class EmAlgorithm extends Algorithm{
    FILE * output_file;
    
    
    int nclasses, niteration;
    
    INPUTDATA_MULTI_GAUSS * create_data(Point p){
        INPUTDATA_MULTI_GAUSS * result = new INPUTDATA_MULTI_GAUSS;
        result->pData = new double[2];
        double data []= {p.getX(), p.getY()};
        memcpy(result->pData, data, sizeof(data));
        result->nClass = -1;
        result->pNormalProb = new double[nclasses];
        return result;
    }
    
    
public:
    
    void setup (char * arg) override {
        sscanf(arg, "%d %d", &nclasses, &niteration);
        output_file = fopen("em.plot", "w");
    }
    
    std::vector<Cluster> find(std::vector<Point> data) override {
        std::vector<Cluster> result;
        CGMM * pGmm = new CGMM();
        INPUTDATA_MULTI_GAUSS ** ppInputData;
        
        ppInputData = new INPUTDATA_MULTI_GAUSS*[data.size()];
        for(int i = 0; i < data.size(); i++){
            ppInputData[i] = create_data(data[i]);
        }
        pGmm->init(nclasses, (int)data.size(), 2, ppInputData);
        pGmm->train();
        
        delete pGmm;
        return result;
        
    }
    
    
    
    
};

#endif /* EmAlgorithm_h */
